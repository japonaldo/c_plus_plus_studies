#!/bin/bash

pushd . > /dev/null 2>&1
cd scripts
export SCRIPT_DIR=$PWD
popd >/dev/null 2>&1

if [ "$#" -eq 1 ]
then
  # TODO improve it, this hardcoded dependency
  if [ "$1" == "0" ]
  then
    source $SCRIPT_DIR/targets/fujioka-x86_64-linux-gnu.sh
  #elif [ "$1" == "1" ]
  #then
  # add new targets
  else
    export JMF_TARGET_BRAND=""
  fi
fi

if [ "$JMF_TARGET_BRAND" == "" ]; then
  echo "No target specified, using default: 0"
  source $SCRIPT_DIR/targets/fujioka-x86_64-linux-gnu.sh
fi

bash --init-file $SCRIPT_DIR/configure_bash.sh

echo "Goodbye!"