import React, { useState } from "react";
import axios from "axios";

export const TimerInput = () => {
  const [hours, setHours] = useState(0);
  const [minutes, setMinutes] = useState(0);
  const [seconds, setSeconds] = useState(0);

  const handleSetTimer = async () => {
    try {
      const now = new Date();
      const futureTime = new Date(
        now.getFullYear(),
        now.getMonth(),
        now.getDate(),
        now.getHours() + parseInt(hours),
        now.getMinutes() + parseInt(minutes),
        now.getSeconds() + parseInt(seconds)
      );
      const body = {
        type: "command",
        control: "buzzer",
        time: futureTime.getTime().toString(),
      };
      await axios.post("/api", body);
    } catch {
      console.log("Error Setting Alarm");
    }
  };

  const handleMinutesChange = (e) => {
    const value = Math.min(59, Math.max(0, e.target.value));
    setMinutes(value);
  };

  const handleSecondsChange = (e) => {
    const value = Math.min(59, Math.max(0, e.target.value));
    setSeconds(value);
  };

  return (
    <div className="flex items-center gap-4">
      <div>
        <label>
          Hours:
          <input
            type="number"
            value={hours}
            onChange={(e) => setHours(e.target.value)}
            className="ml-2 p-1 border rounded"
          />
        </label>
      </div>
      <div>
        <label>
          Minutes:
          <input
            type="number"
            value={minutes}
            onChange={handleMinutesChange}
            max="59"
            className="ml-2 p-1 border rounded"
          />
        </label>
      </div>
      <div>
        <label>
          Seconds:
          <input
            type="number"
            value={seconds}
            onChange={handleSecondsChange}
            max="59"
            className="ml-2 p-1 border rounded"
          />
        </label>
      </div>
      <button
        onClick={handleSetTimer}
        className="ml-4 p-2 bg-green-400 text-black font-medium rounded"
      >
        Set Timer
      </button>
    </div>
  );
};

export default TimerInput;
