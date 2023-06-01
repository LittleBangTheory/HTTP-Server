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
