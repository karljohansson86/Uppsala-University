var sudoku = [];
var outputSudoku = function() {
  sudoku = [];
  $('.square').each(function(i, el) {
    var value = (el.value === "") ? 0 : parseInt(el.value, 10);
    if (isNaN(value) || value < 0 || value > 9) {
      sudoku = [];
      alert('Invalid number at cell: ' + (i+1));
      return false;
    }
    sudoku.push(value);
  });
  if (sudoku.length > 0) {
    $('a').text('OPEN RESULT');
  }
};

$('a').on('click', function() {
  if (sudoku.length > 0) {
    window.open("data:json;charset=utf-8," + encodeURIComponent(JSON.stringify(sudoku)), "_blank");
    $('a').text('GENERATE');
    sudoku = [];
  } else {
    outputSudoku();
  }
});
