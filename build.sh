#!/usr/bin/env sh
# Usage: ./build.sh [debug|release|web|serve]
set -e
cd "$(dirname "$0")"

TARGET="${1:-debug}"

# Where the wasm build of raylib lives (libraylib.web.a + headers).
RAYLIB_WEB_DIR="${RAYLIB_WEB_DIR:-vendor/raylib-web}"
# Sourced only if emcc is not already on PATH. Override if emsdk lives elsewhere.
EMSDK_ENV="${EMSDK_ENV:-$HOME/emsdk/emsdk_env.sh}"

CXXFLAGS="-std=c++20 -Wall -Wextra"

case "$TARGET" in

debug)
	# -fsanitize catches the out-of-bounds reads C# would have thrown on.
	c++ $CXXFLAGS -g -O0 -fsanitize=address,undefined \
		main.cpp -o main -lraylib
	echo "built ./main (debug, sanitized)"
	;;

release)
	c++ $CXXFLAGS -O2 -DNDEBUG \
		main.cpp -o main -lraylib
	echo "built ./main (release)"
	;;

web)
	if ! command -v emcc >/dev/null 2>&1; then
		[ -f "$EMSDK_ENV" ] || { echo "emcc not found and no emsdk at $EMSDK_ENV" >&2; exit 1; }
		# shellcheck disable=SC1090
		. "$EMSDK_ENV" >/dev/null
	fi
	[ -f "$RAYLIB_WEB_DIR/libraylib.web.a" ] || {
		echo "missing $RAYLIB_WEB_DIR/libraylib.web.a" >&2
		echo "build it with: make -C <raylib>/src PLATFORM=PLATFORM_WEB GRAPHICS=GRAPHICS_API_OPENGL_ES3" >&2
		exit 1
	}

	rm -rf build/web
	mkdir -p build/web/stage/static/shaders

	# WebGL 2 speaks GLSL ES 300, not GLSL 330. Rewrite the version line and add
	# the precision qualifier ES requires; the rest of the syntax is shared.
	for f in static/shaders/*; do
		sed '1s|^#version 330.*|#version 300 es\nprecision highp float;|' "$f" \
			> "build/web/stage/static/shaders/$(basename "$f")"
	done
	cp -r static/textures build/web/stage/static/

	emcc main.cpp -o build/web/index.html \
		-std=c++20 -Os \
		-DPLATFORM_WEB \
		-I "$RAYLIB_WEB_DIR/include" "$RAYLIB_WEB_DIR/libraylib.web.a" \
		-sUSE_GLFW=3 \
		-sASYNCIFY \
		-sALLOW_MEMORY_GROWTH=1 \
		-sMIN_WEBGL_VERSION=2 \
		-sMAX_WEBGL_VERSION=2 \
		-sFORCE_FILESYSTEM=1 \
		--preload-file build/web/stage/static@/static
	echo "built build/web/index.html — run './build.sh serve'"
	;;

serve)
	echo "http://localhost:8080/"
	cd build/web && exec python3 -m http.server 8080
	;;

*)
	echo "usage: $0 [debug|release|web|serve]" >&2
	exit 1
	;;
esac
