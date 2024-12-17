import React, { useState } from "react";
import axios from "axios";
import { ClipLoader } from "react-spinners";
import TimerInput from "./TimerInput";

const news_api_key = import.meta.env.VITE_NEWS_API_KEY;

function App() {
  const [didGreetings, setDidGreetings] = useState(false);
  const [newsLoading, setNewsLoading] = useState(false);
  const [greetLoading, setGreetLoading] = useState(false);
  const [isLedOnLoading, setIsLedOnLoading] = useState(false);
  const [timerLoading, setTimerLoading] = useState(false);
  const [isLedOn, setIsLedOn] = useState(false);

  const handleGreetMirror = async () => {
    try {
      setGreetLoading(true);
      const body = {
        type: "greeting",
        timestamp: Date.now().toString(),
      };
      await axios.post("/api", body);
      setDidGreetings(true);
    } catch {
      console.log("Error greeting mirror");
    } finally {
      setGreetLoading(false);
    }
  };

  const handleToggleLed = async () => {
    try {
      setIsLedOnLoading(true);
      const body = {
        type: "command",
        control: "led",
        value: isLedOn ? "off" : "on",
      };
      await axios.post("/api", body);
      setIsLedOn((prev) => !prev);
    } catch {
      console.log("Error toggling LED");
    } finally {
      setIsLedOnLoading(false);
    }
  };

  const handleGetNews = async () => {
    const source = "al-jazeera-english";
    const url = `https://newsapi.org/v2/top-headlines?sources=${source}&apiKey=${news_api_key}`;
    try {
      setNewsLoading(true);
      const response = await axios.get(url);
      const results = [];
      response.data.articles.forEach((article) => {
        const { title, description, author } = article;
        results.push({ title, description, author });
      });

      for (const result of results) {
        const body = {
          ...result,
          type: "data",
        };
        await axios.post("/api", body);
      }
    } catch (err) {
      console.log("Error getting news: ", err);
    } finally {
      setNewsLoading(false);
    }
  };

  // const handleSetTimer = async () => {
  //   try {
  //     setTimerLoading(true);
  //     const body = {
  //       type: "command",
  //       control: "buzzer",
  //       time: (Date.now() + 5 * 1000).toString(),
  //     };
  //     await axios.post("/api", body);
  //   } catch {
  //     console.log("Error setting timer");
  //   } finally {
  //     setTimerLoading(false);
  //   }
  // };

  return (
    <div className="w-full flex flex-col items-center pt-6 gap-7 bg-black h-screen">
      <h1 className="text-white underline font-medium text-3xl">
        Smart Mirror
      </h1>
      <div className="flex gap-5 justify-center">
        {!didGreetings && (
          <button
            className="bg-red-400 px-4 py-2 rounded-xl font-medium text-lg"
            onClick={handleGreetMirror}
            disabled={greetLoading}
          >
            {greetLoading ? <ClipLoader /> : "Greet Mirror"}
          </button>
        )}
        {didGreetings && (
          <div className="flex flex-col gap-5">
            <button
              className="bg-green-400 px-4 py-2 rounded-xl font-medium text-lg"
              onClick={handleGetNews}
              disabled={newsLoading}
            >
              {newsLoading ? <ClipLoader /> : "Get News"}
            </button>
            <button
              className="bg-green-400 px-4 py-2 rounded-xl font-medium text-lg"
              onClick={handleToggleLed}
              disabled={isLedOnLoading}
            >
              {isLedOn ? "Turn Off LED" : "Turn On LED"}
            </button>
            {/* <button
              className="bg-green-400 px-4 py-2 rounded-xl font-medium text-lg"
              onClick={handleSetTimer}
            >
              {timerLoading ? <ClipLoader /> : "Set Timer in 5 seconds"}
            </button> */}
            <TimerInput />
          </div>
        )}
      </div>
      {didGreetings && (
        <div className="p-40  bg-sky-300 border-amber-900 border-[20px] shadow-xl shadow-white"></div>
      )}
    </div>
  );
}

export default App;
