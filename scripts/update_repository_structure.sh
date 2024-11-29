#!/bin/bash

tree -I '.git|build|vcpkg' -L 3 > documentation/repository_structure_tree.txt 