#!/bin/sh
set -eu
#   method: established
#   $1: short_channel_id
#   $2: node_id
#   $3: our_msat
#   $4: funding_txid
DATE=`date -u +"%Y-%m-%dT%H:%M:%S.%N"`
cat << EOS | jq
{ "method":"established", "short_channel_id":"$1", "node_id":"$2", "date":"$DATE", "our_msat":$3, "funding_txid":"$4" }
EOS

## our_msat
#echo $3 > our_msat_$1.txt
