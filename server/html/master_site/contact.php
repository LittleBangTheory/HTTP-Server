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
				<li><a href="about.php">About</a></li>
				<li><a href="contact.php">Contact</a></li>
				<li><a href="dir.php">Directory</a></li>
			</ul>
		</nav>
	</header>
	
	<main>
		<h1>Contact Me</h1>
		<p>You can reach me by email at ProjetHTTP@example.com</p>

		<h1>Allow me to contact you</h1>
        <h2>You can use the GET method :</h2>
		<form method="GET" action="/contact.php">
			<label for="name">Name:</label>
			<input type="text" id="name" name="name" required>
			
			<label for="email">Email:</label>
			<input type="email" id="email" name="email" required>

            <button type="submit">Submit</button>
		</form>
        <h2>Or the POST method :</h2>
        <form method="POST" action="/contact.php">
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
            elseif ($_SERVER['REQUEST_METHOD'] === 'GET' && !empty($_GET['name']) && !empty($_GET['email'])) {
                $name = $_GET['name'];
                $email = $_GET['email'];
    
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
