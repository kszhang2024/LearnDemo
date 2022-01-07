import fs from "fs";

export default class Utils {
  constructor() {
    this.wordSet = [];
    this.index = 0;
    this.firstSet = {
      program: ["{"],
      declarationList: ["ε", "int"],
      statementList: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "ε",
      ],
      declarationStat: ["int"],
      statement: ["if", "while", "for", "read", "write", "{", "ID", ";"],
      ifStat: ["if"],
      ifOther: ["else", "ε"],
      whileStat: ["while"],
      forStat: ["for"],
      readStat: ["read"],
      writeStat: ["write"],
      compoundStat: ["{"],
      assignmentExpression: ["ID"],
      assignmentStat: ["ID"],
      boolExpression: ["(", "ID", "NUM"],
      expressionOther: [">", "<", ">=", "<=", "==", "!="],
      arithmeticExpression: ["(", "ID", "NUM"],
      arithmeticExpression1: ["+", "-", "ε"],
      term: ["(", "ID", "NUM"],
      term1: ["*", "/", "ε"],
      factor: ["(", "ID", "NUM"],
    };
    this.followSet = {
      program: ["#"],
      declarationList: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
      ],
      statementList: ["#", "}"],
      declarationStat: [
        "int",
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
      ],
      statement: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      ifStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      ifOther: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      whileStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      forStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      readStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      writeStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      compoundStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      assignmentExpression: [";", ")"],
      assignmentStat: [
        "if",
        "while",
        "for",
        "read",
        "write",
        "{",
        "ID",
        ";",
        "#",
        "}",
        "else",
      ],
      boolExpression: [")", ";"],
      expressionOther: [")", ";"],
      arithmeticExpression: [";", ")", ">", "<", ">=", "<=", "==", "!="],
      arithmeticExpression1: [";", ")", ">", "<", ">=", "<=", "==", "!="],
      term: ["+", "-", ";", ")", ">", "<", ">=", "<=", "==", "!="],
      term1: ["+", "-", ";", ")", ">", "<", ">=", "<=", "==", "!="],
      factor: ["*", "/", "+", "-", ";", ")", ">", "<", ">=", "<=", "==", "!="],
    };
  }

  readData() {
    let data = fs.readFileSync("D:\\AllProject\\bianyi\\ll1\\dataout.txt", "utf-8");

    for (let item of data.split("\n").slice(1, -1)) {
      this.wordSet.push(item.split("  "));
    }

    this.wordSet.push(["#", "#", "last"]);

    // console.log(this.wordSet);
  }

  getNextSymbol() {
    let data = this.wordSet[this.index++];

    if (data[1] === "annotation") {
      data = this.wordSet[this.index++];
    }

    return data[1];
  }

  error(errMsg) {
    if (errMsg) {
      console.log(errMsg);
    }
    console.log(`不合法的行： ${this.wordSet[this.index - 1][2]}`, `不合法单词: ${this.wordSet[this.index - 1][0]}`);

    process.exit(1);
  }

  existElement(allWordSet, word) {
    return allWordSet.indexOf(word) !== -1;
  }

  first(word) {
    return this.firstSet[word];
  }

  follow(word) {
    return this.followSet[word];
  }
}
