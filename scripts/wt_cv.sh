#!/bin/bash

chmod -R 777 ./containers/

sh scripts/wt_builder.sh
docker build -t wt_cv:latest -f dockerfiles/wt_cv .