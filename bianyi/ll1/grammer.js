import Utils from "./utils.js";

// 定义全局变量
let utils;
let ch;

// 定义错误枚举
const errorSet = [
  "此处应为 {",
  "此处缺少 }",
  "此处应为 ID",
  "此处缺少关键字 int",
  "此处缺少关键字 if",
  "此处应为 (",
  "此处缺少 )",
  "此处缺少关键字 while",
  "此处缺少关键字 for",
  "此处缺少 ;",
  "此处缺少关键字 read",
  "此处缺少关键字 write",
  "此处缺少 =",
  "此处缺少逻辑运算符",
];

function program() {
  if (ch === "{") {
    ch = utils.getNextSymbol();
    declarationList();
    statementList();
    if (ch === "}") {
      ch = utils.getNextSymbol();
    } else {
      utils.error(errorSet[1]);
    }
  } else {
    utils.error(errorSet[0]);
  }
}

function declarationList() {
  if (utils.existElement(utils.first("declarationStat"), ch)) {
    declarationStat();
    declarationList();
  } else if (utils.existElement(utils.follow("declarationList"), ch)) {
    return;
  } else {
    utils.error();
  }
}

function statementList() {
  if (utils.existElement(utils.first("statement"), ch)) {
    statement();
    statementList();
  } else if (utils.existElement(utils.follow("statementList"), ch)) {
    return;
  } else {
    utils.error();
  }
}

function declarationStat() {
  if (ch === "int") {
    ch = utils.getNextSymbol();
    if (ch === "ID") {
      ch = utils.getNextSymbol();
      if(ch === ";") {
        ch = utils.getNextSymbol();
      } else {
        utils.error(errorSet[9]);
      }
    } else {
      utils.error(errorSet[2]);
    }
  } else {
    utils.error(errorSet[3]);
  }
}

function statement() {
  if (utils.existElement(utils.first("ifStat"), ch)) {
    ifStat();
  } else if (utils.existElement(utils.first("whileStat"), ch)) {
    whileStat();
  } else if (utils.existElement(utils.first("forStat"), ch)) {
    forStat();
  } else if (utils.existElement(utils.first("readStat"), ch)) {
    readStat();
  } else if (utils.existElement(utils.first("writeStat"), ch)) {
    writeStat();
  } else if (utils.existElement(utils.first("compoundStat"), ch)) {
    compoundStat();
  } else if (utils.existElement(utils.first("assignmentStat"), ch)) {
    assignmentStat();
  } else if (ch === ";") {
    ch = utils.getNextSymbol();
  } else {
    utils.error();
  }
}

function ifStat() {
  if (ch === "if") {
    ch = utils.getNextSymbol();
    if (ch === "(") {
      ch = utils.getNextSymbol();
      boolExpression();
      if (ch === ")") {
        ch = utils.getNextSymbol();
        statement();
        ifOther();
      } else {
        utils.error(errorSet[6]);
      }
    } else {
      utils.error(errorSet[5]);
    }
  } else {
    utils.error(errorSet[4]);
  }
}

function ifOther() {
  if (ch === "else") {
    // 超前读取一个单词进行判断
    let next = utils.getNextSymbol();

    if (utils.existElement(utils.first("statement"), next)) {
      ch = next;
      statement();
    } else {
      utils.index--;
      return;
    }
  } else if (utils.existElement(utils.follow("ifOther"), ch)) {
    return;
  } else {
    utils.error();
  }
}

function whileStat() {
  if (ch === "while") {
    ch = utils.getNextSymbol();
    if(ch === "(") {
      ch = utils.getNextSymbol();
      boolExpression();
      if (ch === ")") {
        ch = utils.getNextSymbol();
        statement();
      } else {
        utils.error(errorSet[6]);
      }
    } else {
      utils.error(errorSet[5]);
    }
  } else {
    utils.error(errorSet[7]);
  }
}

function forStat() {
  if (ch === "for") {
    ch = utils.getNextSymbol();
    if(ch === "(") {
      ch = utils.getNextSymbol();
      assignmentExpression();
      if (ch === ";") {
        ch = utils.getNextSymbol();
        boolExpression();
        if (ch === ";") {
          ch = utils.getNextSymbol();
          assignmentExpression();
          if (ch === ")") {
            ch = utils.getNextSymbol();
            statement();
          } else {
            utils.error(errorSet[6]);
          }
        } else {
          utils.error(errorSet[9]);
        }
      } else {
        utils.error(errorSet[9]);
      }
    } else {
      utils.error(errorSet[5]);
    }
  } else {
    utils.error(errorSet[8]);
  }
}

function readStat() {
  if (ch === "read") {
    ch = utils.getNextSymbol();
    if(ch === "ID") {
      ch = utils.getNextSymbol();
      if(ch === ";") {
        ch = utils.getNextSymbol();
      } else {
        utils.error(errorSet[9]);
      }
    } else {
      utils.error(errorSet[2]);
    }
  } else {
    utils.error(errorSet[10]);
  }
}

function writeStat() {
  if (ch === "write") {
    ch = utils.getNextSymbol();
    arithmeticExpression();
    if (ch === ";") {
      ch = utils.getNextSymbol();
    } else {
      utils.error(errorSet[9]);
    }
  } else {
    utils.error(errorSet[11]);
  }
}

function compoundStat() {
  if (ch === "{") {
    ch = utils.getNextSymbol();

    statementList();
    if (ch === "}") {
      ch = utils.getNextSymbol();
    } else {
      utils.error(errorSet[1]);
    }
  } else {
    utils.error(errorSet[0]);
  }
}

function assignmentExpression() {
  if (ch === "ID") {
    ch = utils.getNextSymbol();
    if(ch === "=") {
      ch = utils.getNextSymbol();
      arithmeticExpression();
    } else {
      utils.error(errorSet[12]);
    }
  } else {
    utils.error(errorSet[2]);
  }
}

function assignmentStat() {
  if (utils.existElement(utils.first("assignmentExpression"), ch)) {
    assignmentExpression();
    if (ch === ";") {
      ch = utils.getNextSymbol();
    } else {
      utils.error(errorSet[9]);
    }
  } else {
    utils.error();
  }
}

function boolExpression() {
  if (utils.existElement(utils.first("arithmeticExpression"), ch)) {
    arithmeticExpression();
    expressionOther();
  } else {
    utils.error();
  }
}

function expressionOther() {
  if (
    ch === ">" ||
    ch === "<" ||
    ch === ">=" ||
    ch === "<=" ||
    ch === "==" ||
    ch === "!="
  ) {
    ch = utils.getNextSymbol();
    arithmeticExpression();
  } else {
    utils.error(errorSet[13]);
  }
}

function arithmeticExpression() {
  if (utils.existElement(utils.first("term"), ch)) {
    term();
    arithmeticExpression1();
  } else {
    utils.error();
  }
}

function arithmeticExpression1() {
  if (ch === "+") {
    ch = utils.getNextSymbol();
    term();
    arithmeticExpression1();
  } else if (ch === "-") {
    ch = utils.getNextSymbol();
    term();
    arithmeticExpression1();
  } else if (utils.existElement(utils.follow("arithmeticExpression1"), ch)) {
    return;
  } else {
    utils.error();
  }
}

function term() {
  if (utils.existElement(utils.first("factor"), ch)) {
    factor();
    term1();
  } else {
    utils.error();
  }
}

function term1() {
  if (ch === "*") {
    ch = utils.getNextSymbol();
    factor();
    term1();
  } else if (ch === "/") {
    ch = utils.getNextSymbol();
    factor();
    term1();
  } else if (utils.existElement(utils.follow("term1"), ch)) {
    return;
  } else {
    utils.error();
  }
}

function factor() {
  if (ch === "(") {
    ch = utils.getNextSymbol();
    arithmeticExpression();
    if (ch === ")") {
      ch = utils.getNextSymbol();
    } else {
      utils.error(errorSet[6]);
    }
  } else if (ch === "ID") {
    ch = utils.getNextSymbol();
  } else if (ch === "NUM") {
    ch = utils.getNextSymbol();
  } else {
    utils.error();
  }
}

function main() {
  utils = new Utils();
  utils.readData();
  ch = utils.getNextSymbol();
  program();
  if (ch === "#") {
    console.log("语法分析结束, 暂未发现错误");
  }
}

main();