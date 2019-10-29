#!/usr/bin/env bash
vagrant status | grep "not created" | grep -oP "^([a-zA-Z0-9\-])+" | xargs -n 1 -P 4 vagrant up
