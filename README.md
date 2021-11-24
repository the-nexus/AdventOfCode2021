# Advent Of Code C++ Template
This is a C++ project template for the [Advent of Code](https://adventofcode.com/) challenges. 
It simplifies setting up daily challenges code and inputs, and adding new common files to the solution.

## How to use the setup scripts
When adding files to this project, you first need to close Visual Studio and then run the **'generate_solution.sh'** bash script from the command line. This will regenerate the .vcxproj and .vcxproj.filters files of the project solution to add and organize all the files of the solution into Visual Studio without the need for tedious manual drag and drop.

When starting a new daily challenge, you first need to close Visual Studio and then run the **'generate_challenge.sh'** bash script from the command line. This scripts taks a year (YYYY) and a day (DD) as arguments. It will then download the input file from the [Advent of Code](https://adventofcode.com/) website and generate the required code structure for that day to simplify the tedious setup time. The script that downloads the input file needs a session key that matches your Advent Of Code profile in order to download the proper input data.

## Finding your session key
If you need a visual example, please follow [this link](https://github.com/wimglenn/advent-of-code-wim/issues/1).

You'll first need to set up your session key when you first pull this project. To find your session key, open up a webpage containing your input data for any of the days. Then inspect the page and go to the network section to view your cookies (if none of your cookies are visible in that page, reload the webpage by pressing the F5 key).

The session cookie will look something like "cookie: session=9u312097re453490237ty5t4537274h3097a40270" (this is an example key, not a real one). You want to pick what comes after "session=" and place it in the **'session_key.txt'** file that is provided with the **'generate_challange.sh'** and **'generate_solution.sh'** scripts.
