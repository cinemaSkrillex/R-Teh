---
title: Formatting
parent: Developper
nav_order: 1
---
# Code formatting

## Clang-format

This project uses the clang-format tool to enforce a consistent code style. The configuration file is located in the root of the project and is named `.clang-format`.

## Get clang-format

You can install clang-format using the package manager of your choice. For example, on Ubuntu you can install it using the following command:

```bash
sudo apt-get install clang-format
```

## Usage

To format a file, run the following command:

```bash
clang-format -i <file>
```

## Integration with your IDE

Most modern IDEs have a plugin that can automatically format your code using clang-format. For example, in Visual Studio Code you can install the `Clang-Format` extension.

- Settings
- Search "Format"
- Toggle "Format On Save" to true
- Default formatter to "Clang-Format"