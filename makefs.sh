#!/bin/bash

echo "data..."
if ! [ -d data ]; then mkdir data; fi;
echo "conf.."
if ! [ -d conf ] && [ -d ex_conf ]; then cp -r ex_conf conf; fi;
if ! [ -d conf ]; then mkdir conf; fi;
echo "conf/users.."
if ! [ -d conf/users ]; then mkdir conf/users; fi;
