import fs from "fs";
import fetch from "node-fetch";
import "dotenv/config";

const LOGIN = process.env.LOGIN;
const PASSWORD = process.env.PASSWORD;

const DOWNLOAD_PATH_LOCAL = process.env.DOWNLOAD_PATH_LOCAL;
const WEB_DAV_ADDRESS = process.env.WEB_DAV_ADDRESS;

const auth = "Basic " + Buffer.from(`${LOGIN}:${PASSWORD}`).toString("base64");

async function createFolder(folderPath) {
  try {
    const response = await fetch(`${WEB_DAV_ADDRESS}${folderPath}`, {
      method: "MKCOL",
      headers: {
        Authorization: auth,
      },
    });

    if (!response.ok) {
      throw new Error(`Error creating folder! Status: ${response.status}`);
    }

    console.log(`Folder created: ${WEB_DAV_ADDRESS}${folderPath}`);
  } catch (error) {
    console.error("Folder creation error:", error);
  }
}

async function uploadFile(source, destination) {
  try {
    const body = fs.createReadStream(source);
    const response = await fetch(`${WEB_DAV_ADDRESS}${destination}`, {
      method: "PUT",
      headers: {
        Authorization: auth,
        "Content-Type": "application/binary",
      },
      body: body,
    });

    if (!response.ok) {
      throw new Error(`Error uploading file! Status: ${response.status}`);
    }

    console.log(`File uploaded to: ${WEB_DAV_ADDRESS}${destination}`);
  } catch (error) {
    console.error("File upload error:", error);
  }
}

async function downloadFile(path) {
  try {
    const response = await fetch(`${WEB_DAV_ADDRESS}${path}`, {
      method: "GET",
      headers: {
        Authorization: auth,
      },
    });

    if (!response.ok) {
      throw new Error(`Error downloading file! Status: ${response.status}`);
    }

    const buffer = await response.buffer();
    const fileName = getFileName(path);

    fs.writeFileSync(`${DOWNLOAD_PATH_LOCAL}/${fileName}`, buffer);
    console.log(`File downloaded to: ${DOWNLOAD_PATH_LOCAL}/${fileName}`);
  } catch (error) {
    console.error("File download error:", error);
  }
}

async function copyFile(source, destination) {
  try {
    const response = await fetch(`${WEB_DAV_ADDRESS}${source}`, {
      method: "COPY",
      headers: {
        Authorization: auth,
        Destination: `${WEB_DAV_ADDRESS}${destination}`,
        Overwrite: "F",
      },
    });

    if (!response.ok) {
      throw new Error(`Error copying file! Status: ${response.status}`);
    }

    console.log(`File copied to: ${WEB_DAV_ADDRESS}${destination}`);
  } catch (error) {
    console.error("File copy error:", error);
  }
}

async function deleteItem(path) {
  try {
    const response = await fetch(`${WEB_DAV_ADDRESS}${path}`, {
      method: "DELETE",
      headers: {
        Authorization: auth,
      },
    });

    if (!response.ok) {
      throw new Error(`Error deleting item! Status: ${response.status}`);
    }

    console.log(`Item deleted: ${WEB_DAV_ADDRESS}${path}`);
  } catch (error) {
    console.error("Delete error:", error);
  }
}

function getFileName(filePath) {
  return filePath.split("/").pop();
}
//await createFolder("/newFolder");
//await createFolder("/newFolder2");

//await uploadFile("./pic.png", "/newFolder/pic.png");
//await downloadFile("/newFolder/pic.png");
//await copyFile("/pic.png", "/newFolder2/pic.png");
await deleteItem("/pic.png");