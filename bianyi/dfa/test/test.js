const singleChar = /^\(|\)|{|}|;$/;
console.log('/'.match(singleChar));
console.log(singleChar.test('/'));
