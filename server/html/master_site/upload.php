<?php
// Check if the form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Specify the directory where you want to save the uploaded files
    $uploadDir = "uploads/";

    // Create the uploads directory if it doesn't exist
    if (!file_exists($uploadDir)) {
        mkdir($uploadDir, 0777, true);
    }

    // Get the uploaded file details
    $fileName = $_FILES['file']['name'];
    $fileTmpName = $_FILES['file']['tmp_name'];
    $fileSize = $_FILES['file']['size'];
    $fileError = $_FILES['file']['error'];

    // Generate a unique filename to avoid overwriting existing files
    $uniqueFilename = uniqid() . '_' . $fileName;

    // Specify the target path for the uploaded file
    $targetPath = $uploadDir . $uniqueFilename;

    // Check if the file was uploaded successfully
    if ($fileError === UPLOAD_ERR_OK) {
        // Move the temporary file to the target location
        if (move_uploaded_file($fileTmpName, $targetPath)) {
            echo "File uploaded successfully.";
        } else {
            echo "Error moving the uploaded file.";
        }
    } else {
        echo "Error uploading the file.";
    }
}
?>
