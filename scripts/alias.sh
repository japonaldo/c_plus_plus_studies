#!/bin/bash

export JMF_BUILD_BACKEND=${JMF_BUILD_BACKEND:-makefile}

export JMF_CMAKE_BUILD_DBG_DIR=${JMF_PRJ_ROOT_DIR}/.build_${JMF_TARGET_NAME_PLAT}_debug
export JMF_CMAKE_BUILD_DBGFULL_DIR=${JMF_PRJ_ROOT_DIR}/.build_${JMF_TARGET_NAME_PLAT}_debugfull
export JMF_CMAKE_BUILD_RELEASE_DIR=${JMF_PRJ_ROOT_DIR}/.build_${JMF_TARGET_NAME_PLAT}_release

case "$JMF_BUILD_BACKEND" in
	"makefile")
		export JMF_CMAKE_GENERATOR="Unix Makefiles"
		export JMF_BUILD_BACKEND_BIN=make
		;;
	"ninja")
		export JMF_CMAKE_GENERATOR="Ninja"
		export JMF_BUILD_BACKEND_BIN=ninja
		;;
esac

function build_proj {
	SECONDS=0

	if [ $# != 1 ] || ( [ "$1" != "Debug" ] && [ "$1" != "DebugFull" ] && [ "$1" != "Release" ] ); then
		echo -e "usage: build_proj [Release|Debug|DebugFull]\n"
		echo "You can also use one of the following commands as well:"
		echo -e "\tbuild_debug        - equivalent to build_proj Debug"
		echo -e "\tbuild_debugfull    - equivalent to build_proj DebugFull"
		echo -e "\tbuild_release      - equivalent to build_proj Release"
		return
	fi

	export JMF_CMAKE_BUILD_TYPE=$1
	build_type_lc=${JMF_CMAKE_BUILD_TYPE,,}

	pushd . > /dev/null 2>&1

	export JMF_CMAKE_BUILD_DIR=$JMF_PRJ_ROOT_DIR/.build_${JMF_TARGET_NAME_PLAT}_${build_type_lc}

	if [ ! -d $JMF_CMAKE_BUILD_DIR ]; then
		mkdir -p $JMF_CMAKE_BUILD_DIR
	fi

	cd $JMF_CMAKE_BUILD_DIR
	cmake -G "${JMF_CMAKE_GENERATOR}" -DCMAKE_TOOLCHAIN_FILE=$JMF_PRJ_ROOT_DIR/build_cmake_files/Toolchain-${JMF_COMPILER_PREFIX}.cmake -DCMAKE_BUILD_TYPE=$JMF_CMAKE_BUILD_TYPE ..
	${JMF_BUILD_BACKEND_BIN} -j $JMF_COMPILE_CORES
	date

    elapsed_time=$SECONDS

    TZ=UTC0 printf 'Time elapsed: %(%H:%M:%S)T\n' "$elapsed_time"

	popd > /dev/null 2>&1
}

function help_proj {
	echo "Build backend set to: $JMF_BUILD_BACKEND"
	echo "These are the available commands in the current shell:"
	echo "build_proj       - Build current project/product configuration"
	echo "build_debug      - Build current project/product configuration with -DCMAKE_BUILD_TYPE=Debug"
	echo "build_debugfull  - Build current project/product configuration with -DCMAKE_BUILD_TYPE=DebugFull"
	echo "build_clean_debug     - Delete Debug build directory"
	echo "build_clean_debugfull - Delete DebugFull build directory"
	echo "build_clean_release   - Delete Release build directory"
	echo "build_release    - Build current project/product configuration with -DCMAKE_BUILD_TYPE=Release"
	echo "cdroot           - Go to project root directory"
	echo "cdbuild_dbg      - Go to project Debug build directory"
	echo "cdbuild_dbgfull  - Go to project DebugFull build directory"
	echo "cdbuild_release  - Go to project Release build directory"
	echo "deploy_debug     - Equivalent to make install but can be called from any place"
	echo "deploy_debugfull - Equivalent to make install but can be called from any place"
	echo "deploy_release   - Equivalent to make install but can be called from any place"
	echo "cenv             - Equivalent to env | grep JMF_"
	echo ""
}

# ===========================================
# Build aliases
# ===========================================
alias build_debug="build_proj Debug"
alias build_debugfull="build_proj DebugFull"
alias build_release="build_proj Release"
alias build_clean_debug="rm -rf ${JMF_CMAKE_BUILD_DBG_DIR}"
alias build_clean_debugfull="rm -rf ${JMF_CMAKE_BUILD_DBGFULL_DIR}"
alias build_clean_release="rm -rf ${JMF_CMAKE_BUILD_RELEASE_DIR}"

# ===========================================
# Build directories aliases
# ===========================================
alias cdroot="cd $JMF_PRJ_ROOT_DIR"
alias cdbuild_dbg="if [ ! -d ${JMF_CMAKE_BUILD_DBG_DIR} ]; then mkdir -p ${JMF_CMAKE_BUILD_DBG_DIR}; fi; cd ${JMF_CMAKE_BUILD_DBG_DIR}"
alias cdbuild_dbgfull="if [ ! -d ${JMF_CMAKE_BUILD_DBGFULL_DIR} ]; then mkdir -p ${JMF_CMAKE_BUILD_DBGFULL_DIR}; fi; cd ${JMF_CMAKE_BUILD_DBGFULL_DIR}"
alias cdbuild_release="if [ ! -d ${JMF_CMAKE_BUILD_RELEASE_DIR} ]; then mkdir -p ${JMF_CMAKE_BUILD_RELEASE_DIR}; fi; cd ${JMF_CMAKE_BUILD_RELEASE_DIR}"

# ===========================================
# Deploy aliases
# ===========================================
alias deploy_debug="pushd .; cdbuild_dbg; ${JMF_BUILD_BACKEND_BIN} install; popd"
alias deploy_debugfull="pushd .; cdbuild_dbgfull; ${JMF_BUILD_BACKEND_BIN} install; popd"
alias deploy_release="pushd .; cdbuild_release; ${JMF_BUILD_BACKEND_BIN} install; popd"

# ===========================================
# Helper aliases
# ===========================================
alias cenv="env | sort | grep JMF_"
alias help=help_proj
