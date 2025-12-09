#!/usr/bin/env python3
import os
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT = SCRIPT_DIR.parent
TEMPLATE_DIR = SCRIPT_DIR / "templates"

TEMPLATES = {
    "public.h.in": ("include/kernel/{m}.h",),
    "private.h.in": ("src/modules/{m}/private/_{m}.h",),
    "module.c.in": ("src/modules/{m}/{m}.c",),
    "CMakeLists.txt.in": ("src/modules/{m}/CMakeLists.txt",)
}

def read_template(name):
    return (TEMPLATE_DIR / name).read_text()

def render(text, ctx):
    for k, v in ctx.items():
        text = text.replace(f"@{k}@", v)
    return text

def parent_exists(path):
    path.parent.mkdir(parents=True, exist_ok=True)

def write_if_not_exists(path, content, force=False):
    if path.exists() and not force:
        print(f"Skipped existing {path}")
        return
    parent_exists(path)
    path.write_text(content)
    print(f"Created {path}")

def main():
    if len(sys.argv) < 2:
        print("Usage: tools/skeleton.py <module_name> [--force]")
        sys.exit(1)

    mod = sys.argv[1].strip()
    force = "--force" in sys.argv[2:]

    if not mod.isidentifier():
        print("Invalid module name.")
        sys.exit(1)

    ctx = {
        "MODULE_NAME": mod,
        "MODULE_UPPER": mod.upper(),
        "MODULE_UNDERSCORE": "_" + mod,
    }

    for tpl_name, (rel_pattern,) in TEMPLATES.items():
        tpl_text = read_template(tpl_name)
        rendered = render(tpl_text, ctx)
        out_path = ROOT / rel_pattern.format(m=mod)
        write_if_not_exists(out_path, rendered, force=force)

    print(f"Module '{mod}' created.")

if __name__ == "__main__":
    main()
