#!/bin/sh
# Builds cataclysm into out/ with upstream's Makefile. The compiler, flags
# and curses library come from the environment, so one script serves every
# target (see .github/workflows/release.yml):
#
#   asciiverse/build.sh                                         # Debian, Ubuntu
#   LIBS=-lncursesw LDFLAGS="-static -s" asciiverse/build.sh    # Alpine, static
#   CXXFLAGS="-O2 -arch arm64" LDFLAGS="-arch arm64" asciiverse/build.sh  # macOS
set -eu
cd "$(dirname "$0")/.."
CXX="${CXX:-c++}"
CXXFLAGS="${CXXFLAGS:--O2}"
LDFLAGS="${LDFLAGS:-}"
LIBS="${LIBS:--lncurses}"
rm -rf obj cataclysm
mkdir -p out
make -j"${JOBS:-2}" cataclysm \
  CXX="$CXX" WARNINGS= DEBUG= OTHERS="$CXXFLAGS" LDFLAGS="$LDFLAGS $LIBS"
mv cataclysm out/cataclysm
echo "built cataclysm"
