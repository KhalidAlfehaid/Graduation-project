import { initializeApp } from "https://www.gstatic.com/firebasejs/11.6.1/firebase-app.js";
import {
  getDatabase,
  ref,
  onValue,
} from "https://www.gstatic.com/firebasejs/11.6.1/firebase-database.js";

// Firebase Config
const firebaseConfig = {
  apiKey: "AIzaSyAKcmn2DLQk8OArVuS0vqf1X4x3KgODHSU",
  authDomain: "yourspot-system.firebaseapp.com",
  databaseURL:
    "https://parkingspot-65d6f-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "yourspot-system",
  storageBucket: "yourspot-system.appspot.com",
  messagingSenderId: "642355248958",
  appId: "1:642355248958:web:37b82a47debdc6f2cc766f",
  measurementId: "G-X5TEM2HQ74",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

// ربط مع مكان المواقف في Realtime Database
const parkingSlotsRef = ref(db, "parking_slots");

// مكان عرض الكروت
const parkingGrid = document.getElementById("parking-grid");

function renderParkingSpots(spots) {
  parkingGrid.innerHTML = "";

  Object.keys(spots).forEach((key) => {
    const spot = spots[key];

    const spotElement = document.createElement("div");
    spotElement.className = `parking-spot ${spot.status}`;

    const iconType =
      spot.status === "available" ? "check-circle" : "times-circle";
    const statusText = spot.status === "available" ? "متاح" : "مشغول";

    spotElement.innerHTML = `
      <div class="spot-icon">
        <i class="fas fa-${iconType}"></i>
      </div>
      <div class="spot-number">موقف ${key.replace("slot_", "")}</div>
      <div class="spot-status">${statusText}</div>
    `;

    parkingGrid.appendChild(spotElement);
  });
}

// متابعة البيانات الحية
onValue(parkingSlotsRef, (snapshot) => {
  const data = snapshot.val();
  if (data) {
    renderParkingSpots(data);
  }
});
