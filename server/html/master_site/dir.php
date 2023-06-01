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
				<a href="index.html">Home</a>
				<a href="about.php">About</a>
				<a href="contact.php">Contact</a>
                <a href="dir.php">Directory</a>
			</ul>
		</nav>
	</header>
	
	<main>
        <h1>Root directory list :</h1>
    <section>
        <ul>
            <?php
            $arrFiles = array();

            $handle = opendir('/');

            if ($handle) {
                while (($entry = readdir($handle)) !== FALSE) {
                    $arrFiles[] = $entry;
                    echo "<li>$entry</li>";
                }
            }

            closedir($handle);
            ?>
        </ul>
    </section>
	</main>
	
	<footer>
		<p>Copyright © 2023 My Website</p>
	</footer>
</body>
</html>
