# Hawkeye

## Description
A program designed specifically for Linux distributions that monitors process id's received from external programs and restarts said programs when they hang or crash.

## Table of Contents
* [Installation](#installation)
* [Usage](#usage)
* [Credits](#credits)
* [License](./LICENSE)

## Installation
> Built specifically for Raspberry Pi OS (Linux). 

### Make sure Linux is updated

`
sudo apt update && sudo apt upgrade -y
`
### g++, make and git (Latest version)
`
sudo apt install -y g++ make git
`
### libssl-dev (For TCP Communication)
`
sudo apt install -y libssl-dev
`
### mailutils (For email communication)
`
sudo apt install -y mailutils
`

Hawkeye can be download as a zip or cloned down using Git using code menu above

Then make sure to edit `config.cfg` with your personal options and create a secret key to be used by both hawkeye and monitored program for authentication

### [CONFIG.CFG](./config.cfg)

```
secret_key=your_secret_key
log_file=hawkeye.log
# Set to an empty port on host machine
# WILL NOT WORK WITH PORT THAT IS CURRENTLY BEING USED
port=your_port
# 5 by default. Set higher or lower depending on restart tolerance
max_restarts=5
email=youremail@example.com

# You can set as many scripts here as you like
# Set key equal to program name being sent from program being monitored
# Make sure to include full path within value
# Prefix script_ is essential so must be included. ONLY CHANGE program_name and path to script
# (OPTIONAL) Can use included scripts folder to localize all your restart scripts for easier access
script_program_name1=/scripts/your/script.sh
script_program_name2=/scripts/your/script.sh
script_program_name3=/scripts/your/script.sh

# Set these and however many more you want to whatever error you'd like to look for
# Make sure it matches the error message exactly
error_1=Cannot complete handshake
error_2=fatal error
error_3=connection lost
error_4=unexpected token
```


Then run `make` in the terminal to compile the program.

### To run Hawkeye

Navigate to folder where Hawkeye is stored and type:

`./hawkeye`

Terminal should read:

    Listening on PORT [Whatever port you entered in config.cfg]

Now Hawkeye will wait for communication from your monitored programs!

## Usage

Hawkeye simply monitors process ID's from whatever programs you want monitored and will restart them using shell scripts provided. 

Hawkeye will also monitor for hung processes by monitoring the processes error messages as well.

In the `config.cfg`, you can provide a `MAX_RESTART` limit that will tell Hawkeye how many restarts of a program is allowed before it will stop trying. If Hawkeye reaches this limit, it will cease the restart process and send out an email alert to the email provided.

### Code in Monitored program
You're monitored program must include a function that sends it's own PID, Name, and the `secret_key` within Hawkeye for autentication.

Here's an example of the code I built for my Node.JS program:

### UTILS
```
const net = require('net');

hawkeyeConnect: () => {
    const client = new net.Socket();

    const data = `${process.pid} NAME_OF_PROGRAM ${config.hawkeye.hawkeyeKey}`;

    client.connect(config.hawkeye.port, config.hawkeye.host, () => {
      client.write(data);
      client.end();
      console.log("Connected to Hawkeye, sending PID...");
    })

    client.on('error', (e) => {
      const todayDate = new Date().toJSON();
      const msg = `${todayDate}: ${e} ::hawkeye connect::\n`;

      // Log error to file
      fs.appendFile('errors.log', msg, (err) => {
        if (err) console.error(err)
      })
    })
  }
```

### INDEX.JS
```
hawkeyeConnect();
```

### CONFIG
```
{
  "hawkeye": {
    "port": "your_port_here",
    "host": "127.0.0.1",
    "hawkeyeKey": "your_secret_key_here"
  }
}
```
It doesn't matter what language you use. Just be sure to include `data` in the format:

`pid [NAME_OF_PROGRAM] secret_key`

`NAME_OF_PROGRAM` must match the key for the script in your `config.cfg` exactly.

### CONFIG.CFG
`script_NAME_OF_PROGRAM=scripts/path/to/your/script.sh`

Send this to the host and port chosen in JSON format.

## Credits
* [Myself](https://github.com/Opaleone)
* [ChatGPT](https://chatgpt.com/)

## License
[MIT License](https://opensource.org/license/mit)

Also see license in repository for more information.