#!/bin/sh
# Packs the build into dist/cataclysm-<target>.tar.gz for av: the binary at
# the top and the game's read-only data beside it in data/. At start the
# game copies that data into the player's own folder and plays from there
# ($XDG_STATE_HOME/cataclysm, or Application Support on macOS), so nothing
# is written to the install folder.
#
# Left out: data/termfont and data/FONTDATA (a bitmap font and its settings
# for upstream's own Windows window, not used in a terminal) and the
# options.txt and keymap.txt in the repo (the game writes its own).
#
#   asciiverse/package.sh x86_64-unknown-linux-musl
set -eu
cd "$(dirname "$0")/.."
target="$1"
stage=stage/cataclysm
rm -rf "$stage" && mkdir -p "$stage/data" dist
cp out/cataclysm LICENSE "$stage/"
cp README "$stage/README.upstream"
cp asciiverse/README.txt "$stage/README.txt"
for f in motd LAB_NOTES NAMES_FEMALE NAMES_LAST NAMES_MALE NPC_HINTS; do
  cp "data/$f" "$stage/data/"
done
tar -czf "dist/cataclysm-$target.tar.gz" -C "$stage" .
ls dist
