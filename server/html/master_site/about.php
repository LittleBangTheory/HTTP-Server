<!DOCTYPE html>
<html>
<head>
	<title>About</title>
	<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
	<header>
		<nav>
			<ul>
				<a href="index.html">Home</a>
				<a href="about.html">About</a>
				<a href="contact.php">Contact</a>
				<a href="dir.php">Directory</a>
			</ul>
		</nav>
	</header>
	
	<main>
		<h1>About Me</h1>
		<p>Site web de test pour le projet de NE302.</p>
		Ici vous pouvez <a href="medias/NE302_Projet_Etape3.pdf">télécharger</a> le fichier de consignes pour l'étape 3.
		<form method="POST" action="upload.php" enctype="multipart/form-data">
			<label for="file">Select a file to upload:</label>
			<input type="file" id="file" name="file" accept=".txt, .pdf, .docx"> <!-- Specify accepted file types using the accept attribute -->
			<button type="submit">Upload</button>
		</form>
		
	</main>
	
	<footer>
		<p>Copyright © 2023 My Website</p>
	</footer>
</body>
</html>
