#!/bin/bash

if [ -e ~/.bashrc ]; then source ~/.bashrc; fi

export JMF_COMPILE_CORES=${JMF_COMPILE_CORES:-10}
export JMF_PRJ_ROOT_DIR=$PWD

PROMPT_COMMAND=prompt_command

source $SCRIPT_DIR/color.sh
source $SCRIPT_DIR/alias.sh

prompt_command () {  
	PS1="\[$BGreen\]\u@\h\[$txtrst\]:\[$BBlue\]\W\[$txtrst\][\[$BCyan\]${JMF_TARGET_NAME}:$JMF_TARGET_PLAT\[$txtrst\]]\$ "
}
