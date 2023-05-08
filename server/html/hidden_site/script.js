// A function to generate a random number between min and max (inclusive)
function getRandomNumber(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }
  
  // A function to generate a random color
  function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }
  
  // Get the HTML elements to modify
  var section = document.querySelector('section');
  var h2 = document.querySelector('h2');
  var p = document.querySelector('p');
  
  // Generate a random number and set it as the header's font size
  var randomFontSize = getRandomNumber(20, 60);
  h2.style.fontSize = randomFontSize + 'px';
  
  // Generate a random color and set it as the header's text color
  var randomColor = getRandomColor();
  h2.style.color = randomColor;
  
  // Generate a random number and set it as the section's height
  var randomHeight = getRandomNumber(200, 600);
  section.style.height = randomHeight + 'px';
  
  // Generate a random color and set it as the section's background color
  var randomBackgroundColor = getRandomColor();
  section.style.backgroundColor = randomBackgroundColor;
  
  // Generate a random number and set it as the paragraph's font size
  var randomFontSize = getRandomNumber(12, 24);
  p.style.fontSize = randomFontSize + 'px';
  
  // Generate a random color and set it as the paragraph's text color
  var randomColor = getRandomColor();
  p.style.color = randomColor;
  
  // Log a message to the console to indicate that the script has finished executing
  console.log('JavaScript script has finished executing');