<!DOCTYPE html>
<html>
<head>
	<title>Contact</title>
	<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
	<header>
		<nav>
			<ul>
				<li><a href="index.html">Home</a></li>
				<li><a href="about.html">About</a></li>
				<li><a href="contact.html">Contact</a></li>
			</ul>
		</nav>
	</header>
	
	<main>
		<h1>Contact Me</h1>
		<p>You can reach me by email at john@example.com or by phone at 555-555-5555.</p>

		<h1>Allow me to contact you</h1>
		<form method="POST" action="/submit-form">
			<label for="name">Name:</label>
			<input type="text" id="name" name="name" required>
			
			<label for="email">Email:</label>
			<input type="email" id="email" name="email" required>
			
			<button type="submit">Submit</button>
		</form>		  
		
		<p>The name and email values will be printed to the terminal !</p>
		<?php
			if ($_SERVER['REQUEST_METHOD'] === 'POST') {
				$name = $_POST['name'];
				$email = $_POST['email'];

				echo "<h2>Submitted Data:</h2>";
				echo "<p>Name: " . $name . "</p>";
				echo "<p>Email: " . $email . "</p>";
			}
		?>
	</main>
	
	<footer>
		<p>Copyright © 2023 My Website</p>
	</footer>
</body>
</html>
