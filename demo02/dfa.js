const fs = require("fs");
const readline = require("readline");

/**
 * 字符串类别映射
 */
const kind = {
  "0": "种类",
  "1": "标识符",
  "2": "无符号整数",
  "3": "分界符",
  "4": "运算符",
  "5": "注释",
  "6": "保留字"
}

/**
 * 写入文件函数
 * @param {*} strOutputPath 写入路径
 * @param {*} str 合法字符串
 * @param {*} kindNo 种类ID
 */
function writeData(strOutputPath, str, kindNo) {
  let content = str + "  " + kind[kindNo] + "\n";

  try {
    fs.appendFileSync(strOutputPath, content);
  } catch(e) {
    console.log("appendFileError: " + e.message);
  }
}

/**
 * 处理错误函数
 * @param {*} lineNo 错误行码 
 * @param {*} str 错误的字符串
 * @param {*} local 错误的字符
 */
function error(lineNo, str, local) {
  console.log(`Unaccepted String:${str} in ${lineNo}`);
  console.log(`Illegal Char:${local}`);
}

/**
 * DFA分析程序
 */
function dfa(strInputFilePath, strOutputPath) {
  if (!strInputFilePath || !strOutputPath) {
    console.log("Please give input-file or output-file");
  }
  
  // 匹配正则
  const isAlpha = /^[A-Za-z]$/;
  const isNumber = /^[0-9]$/;
  const notZeroNum = /^[1-9]$/;
  const singleChar = /^\(|\)|\{|\}|;$/;
  const singleOper = /^\+|\-|\*$/;
  const doubleOper = />|<|=|!/;
  const keyWords = ["if", "else", "for", "while", "int", "write", "read"];
  let state = 0;

  try {
    const fRead = fs.createReadStream(strInputFilePath);
    const fWrite = fs.createWriteStream(strOutputPath);

    const txtReadLine = readline.createInterface({
      input: fRead,
      output: fWrite,
      terminal: false,
    });
    let buf1 = "";  // 存储即将要写入的单词符号
    let lineNo = 0; // 记录行号

    // 写入第一行
    writeData(strOutputPath, "单词符号", "0");

    // 按行读取数据
    txtReadLine.on("line", (txtLineData) => {
      lineNo++;

      for (let i=0; i<txtLineData.length; i++) {
        // console.log(buf1);

        let txtData = txtLineData[i];

        switch (state) {
          case 0:
            if (isAlpha.test(txtData)) {
              buf1 += txtData;
              state = 1;
            } else if (notZeroNum.test(txtData)) {
              buf1 += txtData;
              state = 2;
            } else if (txtData === '0') {
              buf1 = txtData;
              writeData(strOutputPath, buf1, "2");
              buf1 = "";
            } else if (singleChar.test(txtData)) {
              buf1 = txtData;
              writeData(strOutputPath, buf1, "3");
              buf1 = "";
            } else if (singleOper.test(txtData)) {
              buf1 = txtData;
              writeData(strOutputPath, buf1, "4");
              buf1 = "";
            } else if (doubleOper.test(txtData)) {
              buf1 = txtData;
              state = 3;
            } else if (txtData === "/") {
              buf1 += txtData;
              state = 4;
            } else if (txtData !== " ") {
              error(lineNo, txtData, txtData);
            }
            break;
          case 1:
            if (isAlpha.test(txtData) || isNumber.test(txtData)) {
              buf1 += txtData;
            } else {
              if (keyWords.indexOf(buf1) !== -1) {
                writeData(strOutputPath, buf1, "6");
              } else {
                writeData(strOutputPath, buf1, "1");
              }
              buf1 = "";
              state = 0;
              // 重新读取当前字符
              i--;
            }
            break;
          case 2:
            if (isNumber.test(txtData)) {
              buf1 += txtData;
            } else {
              writeData(strOutputPath, buf1, "2");
              buf1 = "";
              state = 0;
              // 重新读取当前字符
              i--;
            }
            break;
          case 3:
            if (txtData === '=') {
              buf1 += txtData;
              writeData(strOutputPath, buf1, "4");
              buf1 = "";
              state = 0;
            } else {
              if(/^<|>|=$/.test(buf1)) {
                writeData(strOutputPath, buf1, "4");
                // 重新读取当前字符
                i--;
              } else {
                error(lineNo, buf1+txtData, txtData);
              }
              state = 0;
              buf1 = "";
            }
            break;
          case 4:
            switch (txtData) {
              case "*":
                buf1 += txtData;
                state = 5;
                break;
              default:
                writeData(strOutputPath, buf1, "4");
                buf1="";
                state = 0;
            }
            break;
          case 5:
            switch (txtData) {
              case "*":
                buf1 += txtData;
                state = 6;
                break;
              default:
                buf1 += txtData;
            }
            break;
          case 6:
            switch (txtData) {
              case "/":
                buf1 += txtData;
                writeData(strOutputPath, buf1, "5");
                buf1 = "";
                state = 0;
              case "*":
                buf1 += txtData;
                break;
              default:
                buf1 += txtData;
                state = 5;
            }
        }
      }

      // 遍历完一行之后，状态没有回到初态。
      if(state !== 0) {
        error(lineNo, buf1, buf1);
        state = 0;
        buf1 = "";
      }
    });
  } catch (e) {
    console.log("error: " + e.message);
  }
}

dfa("./datain.txt", "./dataout.txt");
