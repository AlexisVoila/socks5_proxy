# socks5 proxy server
___
Socks5 proxy server implementation

## Dependencies
___
* CMake
* C++ 17 
* C++ Boost

## Build instructions
#### Linux
    $ git clone https://github.com/alexisvoila/socks5_proxy.git
    $ cd socks5_proxy
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make   

## Program options
___
```
All options:
  -p [ --port ] arg              socks5 server listen port number
  -v [ --log_level ] arg (=info) verbosity level of log messages 
                                 [debug|trace|info|warning|error|fatal]
  -l [ --log_file ] arg          log file path
  -h [ --help ]                  show help message
  
```
## Usage example 
___
```
$ ./socks5_proxy --port=8080 --log_file=demo.log --log_level=info 
```
### Output example
```
2021-06-14 16:26:51.302 [info]	socks5-proxy server starts on port: 8080
2021-06-14 16:30:50.105 [info]	[1] requested [www.google.com:443]
2021-06-14 16:30:50.109 [info]	[1] resolved [www.google.com] --> [142.250.150.147]
2021-06-14 16:30:50.120 [info]	[1] connected to remote address [142.250.150.147:443]
2021-06-14 16:30:50.344 [info]	[2] requested [lh3.googleusercontent.com:443]
2021-06-14 16:30:50.344 [info]	[3] requested [www.gstatic.com:443]
2021-06-14 16:30:50.347 [info]	[2] resolved [lh3.googleusercontent.com] --> [64.233.165.132]
2021-06-14 16:30:50.350 [info]	[3] resolved [www.gstatic.com] --> [108.177.14.94]
2021-06-14 16:30:50.358 [info]	[2] connected to remote address [64.233.165.132:443]
2021-06-14 16:30:50.361 [info]	[3] connected to remote address [108.177.14.94:443]
2021-06-14 16:30:50.571 [info]	[4] requested [apis.google.com:443]
2021-06-14 16:30:50.571 [info]	[5] requested [aa.google.com:443]
2021-06-14 16:30:50.581 [info]	[4] resolved [apis.google.com] --> [74.125.205.196]
2021-06-14 16:30:50.591 [info]	[5] resolved [aa.google.com] --> [142.250.150.101]
2021-06-14 16:30:50.592 [info]	[4] connected to remote address [74.125.205.196:443]
2021-06-14 16:30:50.602 [info]	[5] connected to remote address [142.250.150.101:443]
2021-06-14 16:30:50.911 [info]	[6] requested [play.google.com:443]
2021-06-14 16:30:50.915 [info]	[6] resolved [play.google.com] --> [173.194.220.139]
2021-06-14 16:30:50.917 [info]	[7] requested [play.google.com:443]
2021-06-14 16:30:50.918 [info]	[7] resolved [play.google.com] --> [173.194.220.101]
2021-06-14 16:30:50.926 [info]	[6] connected to remote address [173.194.220.139:443]
2021-06-14 16:30:50.929 [info]	[7] connected to remote address [173.194.220.101:443]
2021-06-14 16:30:54.573 [info]	[8] requested [ogs.google.com:443]
2021-06-14 16:30:54.576 [info]	[8] resolved [ogs.google.com] --> [142.250.150.100]
2021-06-14 16:30:54.587 [info]	[8] connected to remote address [142.250.150.100:443]
2021-06-14 16:30:54.748 [info]	[9] requested [fonts.gstatic.com:443]
2021-06-14 16:30:54.750 [info]	[9] resolved [fonts.gstatic.com] --> [173.194.220.94]
2021-06-14 16:30:54.762 [info]	[9] connected to remote address [173.194.220.94:443]
2021-06-14 16:30:54.762 [info]	[9] session closed, rx_bytes: 0, tx_bytes: 0
2021-06-14 16:30:54.826 [info]	[10] requested [ogs.google.com:443]
2021-06-14 16:30:54.826 [info]	[11] requested [ogs.google.com:443]
2021-06-14 16:30:54.826 [info]	[10] resolved [ogs.google.com] --> [142.250.150.100]
2021-06-14 16:30:54.827 [info]	[11] resolved [ogs.google.com] --> [142.250.150.100]
2021-06-14 16:30:54.838 [info]	[11] connected to remote address [142.250.150.100:443]
2021-06-14 16:30:54.838 [info]	[10] connected to remote address [142.250.150.100:443]
2021-06-14 16:30:54.920 [info]	[10] session closed, rx_bytes: 6277, tx_bytes: 1720
2021-06-14 16:30:59.689 [info]	[12] requested [accounts.google.com:443]
2021-06-14 16:30:59.702 [info]	[12] resolved [accounts.google.com] --> [108.177.14.84]
2021-06-14 16:30:59.713 [info]	[12] connected to remote address [108.177.14.84:443]
2021-06-14 16:30:59.731 [info]	[13] requested [ocsp.pki.goog:80]
2021-06-14 16:30:59.734 [info]	[13] resolved [ocsp.pki.goog] --> [173.194.220.94]
2021-06-14 16:30:59.745 [info]	[13] connected to remote address [173.194.220.94:80]
2021-06-14 16:31:03.364 [info]	[13] session closed, rx_bytes: 701, tx_bytes: 385
2021-06-14 16:31:03.364 [info]	[12] session closed, rx_bytes: 5432, tx_bytes: 2593
2021-06-14 16:31:03.364 [info]	[11] session closed, rx_bytes: 6275, tx_bytes: 1655
2021-06-14 16:31:03.364 [info]	[8] session closed, rx_bytes: 22085, tx_bytes: 1964
2021-06-14 16:31:03.365 [info]	[7] session closed, rx_bytes: 5933, tx_bytes: 1206
2021-06-14 16:31:03.365 [info]	[6] session closed, rx_bytes: 2741, tx_bytes: 7183
2021-06-14 16:31:03.365 [info]	[5] session closed, rx_bytes: 18084, tx_bytes: 1994
2021-06-14 16:31:03.365 [info]	[4] session closed, rx_bytes: 3568, tx_bytes: 2236
2021-06-14 16:31:03.365 [info]	[3] session closed, rx_bytes: 173197, tx_bytes: 3542
2021-06-14 16:31:03.365 [info]	[2] session closed, rx_bytes: 17492, tx_bytes: 1834
2021-06-14 16:31:03.365 [info]	[1] session closed, rx_bytes: 357871, tx_bytes: 5813

```
