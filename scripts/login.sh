#!/usr/bin/env

# Please, make sure base_dir/scripts/ and base_dir/.env
base_dir=/c/Prjs/andromache
_cmd=""

set -e

cd $base_dir &&
source .env

if command -v pacman &> /dev/null ; then
        _cmd="pacman -Q 'expect'"
else
        _cmd="dpkg -s 'expect'"
fi

if eval "$_cmd" &> /dev/null 2>&1; then
        "expect" "$base_dir/scripts/login.exp" "$VM_USER" "$VM_HOST" "$VM_PORT" "$VM_ROOT"

else    
        echo "expect does not exist (hint: pacman -S expect or sudo apt install expect)"

        echo "sudo -p '' su - -c whoami" | ssh -o StrictHostKeyChecking=no $VM_USER@$VM_HOST -p $VM_PORT

        exit 1
fi

exit 0