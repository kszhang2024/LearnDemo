let _ = require('lodash');

console.log(_.flatten([1, [2, [3, [4]], 5]]));
console.log(_.flattenDeep([1, [2, [3, [4]], 5]]));
