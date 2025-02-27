# ctp2db

ctp2db is a command tool to receive data from CTP API and store it in a database. There will not be an executable file
named ctp2db but some executable files named ctp2pg, ctp2mysql, ctp2oracle, ctp2duckdb and so on. 
It's used by investors to do data analysis of Chinese futures market.

# Getting Started

## Prerequisites
You have to have ctp user account and password to login to the CTP API. You also need to have a database to store the data.
You will find test account and password here: https://www.simnow.com.cn/ or https://github.com/openctp/openctp.


## Installation

```shell
# Windows
# Download the zip file from the release page

# Unzip the file (You may need to install unzip first or just use other tools to unzip it)
unzip ctp2pg-0.0.1-win64.zip

# Run the program with the help option to see the usage
cd ctp2pg-0.0.1-win64
bin/ctp2pg.exe -h
```

```shell
# Linux
unzip ctp2pg-0.0.1-Linux.zip
cd ctp2pg-0.0.1-Linux
bin/ctp2pg -h
```

``` text
sink data to postgresql from ctp
Usage: ctp2pg.exe [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -c,--config TEXT REQUIRED   ctp config file with yaml format.
  -p,--pg_conn TEXT REQUIRED  pg connection as: postgresql://user:password@127.0.0.1:5432/test
  -t,--table TEXT             table name of hq data
  -i,--instruments TEXT ... REQUIRED
                              instruments to sink
  -d,--delay INT              delay in ms between each batch of data. Default is 100ms.

```

## Configuration

You need to create a configuration file in yaml format. Here is an example:
```yaml
client_access:
  # 经纪公司代码
  BrokerID: ""
  # 用户代码
  UserID: "1234"
  # 用户端产品信息
  UserProductInfo: "client_xxxxx_1.0.0"
  # 认证码
  AuthCode: ""
  # App代码
  AppID: "client_xxxxx_1.0.0"

user_access:
  # 交易日
  TradingDay: ""
  # 经纪公司代码
  BrokerID: ""
  # 用户代码
  UserID: "1234"
  # 密码
  Password: "guessme"
  # 用户端产品信息
  UserProductInfo: ""
  # 接口端产品信息
  InterfaceProductInfo: ""
  # 协议信息
  ProtocolInfo: ""
  # Mac地址
  MacAddress: ""
  # 动态密码
  OneTimePassword: ""
  # 保留 (这里用来存 InvestorID)
  reserve1: "5601"
  # 登录备注
  LoginRemark: "test"
  # 终端端口
  ClientIPPort: "54321"
  # 终端IP
  ClientIPAddress: "106.14.xxx.75"

connect_info:
  front_hq_address: "tcp://xxx.37.xx.177:20004"


```

## Usage

```shell
# Windows
bin/ctp2pg.exe -c config.yaml -p postgresql://user:password@127.0.0.1:5432/test -i ag2504 au2504
# Linux
bin/ctp2pg -c config.yaml -p postgresql://user:password@127.0.0.1:5432/test -i ag2504 au2504
```

Default table name is t_finance_ctp_hq. You can specify the table name with -t option.

**Data in database** looks like this:

![docs/_static/data-example.png](docs/_static/data-example.png "data-example.png")

**Table create sql** in this fie: ctp2pg/ctp2pg.cpp
