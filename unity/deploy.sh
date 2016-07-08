#!/bin/sh

SCRIPT_DIR="`readlink -f $(dirname $0)`"
SDL_BASE="`readlink -f ${SCRIPT_DIR}/..`"
DEPLOY="${SDL_BASE}/deploy"
echo "Base is ${SDL_BASE}"
echo "Deploying to ${DEPLOY}"

# Sanity check
if test "x${SDL_BASE}" = 'x' || test "x${SDL_BASE}" = 'x/'; then
	echo "Error determining SDL base, abort!"
	exit 1
fi

# Clean slate
rm -rf "${DEPLOY}"
mkdir -p "${DEPLOY}"

# Copy includes
echo "Deploying includes"
cp -R "${SDL_BASE}/include" "${DEPLOY}/"
# TODO: Mechanism for updating non-linux config headers
CONFIG_HEADER="${DEPLOY}/include/SDL_config.h"
for platform in linux32 linux64; do
	platform_include="${DEPLOY}/include/${platform}"
	mkdir -p "${platform_include}"
	cp "${CONFIG_HEADER}" "${platform_include}/"
done
rm -f "${CONFIG_HEADER}"

# Copy source
echo "Copying source"
for source in `cat "${SCRIPT_DIR}/sources.list"`; do
	source_dir="${DEPLOY}/`dirname ${source}`"
	mkdir -p "${source_dir}"
	cp "${SDL_BASE}/${source}" "${source_dir}"
	header=`echo "${source}" | sed 's/\.c$/.h/'`
	if test -e "${SDL_BASE}/${header}"; then
		cp "${SDL_BASE}/${header}" "${source_dir}"
	fi
	cheader=`echo "${source}" | sed 's/\.c$/_c.h/'`
	if test -e "${SDL_BASE}/${cheader}"; then
		cp "${SDL_BASE}/${cheader}" "${source_dir}"
	fi
done

# Generate jamfiles
echo "Generating jamfiles"
SOURCES=`cat "${SCRIPT_DIR}/sources.list"`
JAMFILE="${DEPLOY}/SDL2_sources.jam"
echo "SDL_SOURCES =" > "${JAMFILE}"
echo "${SOURCES}" >> "${JAMFILE}"
echo ';' >> "${JAMFILE}"
