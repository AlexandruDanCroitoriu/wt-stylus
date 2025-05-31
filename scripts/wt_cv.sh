#!/bin/bash

sh scripts/wt_builder.sh
docker build -t wt_cv:latest -f dockerfiles/wt_cv .