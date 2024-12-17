import express from "express";
import bodyParser from "body-parser";

const app = express();
const port = 3000;
const host = "0.0.0.0";

app.use(bodyParser.json());

app.get("/", (req, res) => {
  console.log("GET request received");
  res.send("Hello Mirror Server");
});

app.post("/", (req, res) => {
  console.log(req.body);
  res.send("Post request received");
});

app.listen(port, host, () => {
  console.log(`Server is running on http://${host}:${port}`);
});
