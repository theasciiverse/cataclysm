#!/bin/sh
# Records asciiverse/cataclysm.cast, the catalog preview, at 100x30
# (hub/RECORDING.md) with hub/tools/castrec.py from a sibling hub checkout:
# the title screen, a random character, the evacuation shelter, the walk
# out into the fields, the overmap, and on west towards the houses. The game plays from a
# throwaway state folder; the build in out/ is used (asciiverse/build.sh).
#
#   sh asciiverse/record.sh
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
castrec=${CASTREC:-$root/../../hub/tools/castrec.py}
inst=$(mktemp -d)
state=$(mktemp -d)
trap 'rm -rf "$inst" "$state"' EXIT
mkdir -p "$inst/data"
cp "$root/out/cataclysm" "$inst/"
for f in motd LAB_NOTES NAMES_FEMALE NAMES_LAST NAMES_MALE NPC_HINTS; do
  cp "$root/data/$f" "$inst/data/"
done
script="w3;kj;w0.6;kenter;w1;kj;w0.4;kj;w0.4;kenter;w2.5;k?;w1.2;k>;w1;kY;w4"
script="$script;r6:j:0.35;w0.6;r14:h:0.3;w1.5;r10:k:0.35;w1;r12:h:0.3;w1.5"
script="$script;km;w4;kesc;w1;r45:h:0.15;w1;r20:y:0.18;w1.5;r20:h:0.15;w2.5"
XDG_STATE_HOME=$state python3 "$castrec" "$root/asciiverse/cataclysm.cast" "$state" "$script" -- "$inst/cataclysm"
