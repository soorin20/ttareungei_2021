const submitbutton = document.querySelector("#submitbutton");
const destination = document.querySelector("#destination");
const totcnt = document.querySelector("#parkingbiketotcnt");
const racktotcnt = document.querySelector("#racktotcnt");
const shared = document.querySelector("#shared");
const stationid = document.querySelector("#statonid");
const stationlat = document.querySelector("#latitude");
const stationlng = document.querySelector("#longitude");
const stationname = document.querySelector("#stationname");
const inputplace = document.querySelector("#inputplace");
const container = document.querySelector("#map"); //지도를 담을 영역의 DOM 레퍼런스
const informandbutton = document.querySelector("#informandbutton");
const mapandbutton = document.querySelector("#mapandbutton");
const url = `http://openapi.seoul.go.kr:8088/4843576e436b616e36386d79637671/json/bikeList/1/1000/`;

function matchwithAPI(event) {
  event.preventDefault();
  console.dir(destination);
  const stationID = destination.value;
  console.log(stationID);
  inputplace.classList.add("active");
  fetch(url)
    .then((response) => response.json())
    .then((data) => {
      const datarow = data.rentBikeStatus.row;
      console.log(datarow);
      console.log(datarow[0].stationId);
      console.log(`ST-${stationID}`);
      const matchedstation = Object.keys(datarow).filter((key) =>
        [`ST-${stationID}`].includes(datarow[key].stationId)
      );
      const matchedstationinform = datarow[matchedstation];
      console.log(matchedstation);
      console.log(matchedstationinform);
      console.log(matchedstationinform.parkingBikeTotCnt);
      const button = document.createElement("button");
      const information = document.querySelector("#information");
      button.innerText = "Your destination";
      button.id = "informbutton";
      informandbutton.insertBefore(button, information);
      stationname.innerText = `대여소의 이름은 ${matchedstationinform.stationName}입니다.`; // 대여소 이름
      totcnt.innerText = `현재 거치된 자전거는 ${matchedstationinform.parkingBikeTotCnt}대 입니다.`; // 현재 몇 개의 자전거 거치되어 있는가?
      racktotcnt.innerText = `총 거치대 개수는 ${matchedstationinform.rackTotCnt}대 입니다.`; // 해당 대여소의 거치대 개수
      shared.innerText = `현재 ${matchedstationinform.shared}% 의 자전거가 거치되었습니다.`; // 거치율(퍼센트)
      stationid.innerText = `대여소의 ID는 ${matchedstationinform.stationId}입니다.`; // 대여소 ID
      stationlat.innerText = `대여소의 위도는 ${matchedstationinform.stationLatitude}입니다.`; // 대여소 위도
      stationlng.innerText = `대여소의 경도는 ${matchedstationinform.stationLongitude}입니다.`; // 대여소 경도
      const mapbutton = document.createElement("button");
      mapbutton.innerText = "Map";
      mapbutton.id = "mapbutton";
      mapandbutton.insertBefore(mapbutton, container);
      console.log(matchedstationinform.stationLatitude);
      let distarray = [];

      for (i = 0; i < 1000; i++) {
        let distance = getdist();
        let distarraylat = datarow[i].stationLatitude;
        let distarraylng = datarow[i].stationLongitude;
        let distarrayshared = datarow[i].shared;
        let distarrayname = datarow[i].stationName;

        distarray.push({
          number: i,
          dist: distance,
          lat: distarraylat,
          lng: distarraylng,
          shared: distarrayshared,
          name: distarrayname,
        });
      }

      const arrangeddistdarray = distarray.sort(function (a, b) {
        return a.dist - b.dist;
      });

      const splicedarray = arrangeddistdarray.slice(1, 11);
      console.log(splicedarray);

      const options = {
        //지도를 생성할 때 필요한 기본 옵션
        center: new kakao.maps.LatLng(
          matchedstationinform.stationLatitude,
          matchedstationinform.stationLongitude
        ), // 지도의 기본 중심
        level: 5, //지도의 레벨(확대, 축소 정도)
      };

      const map = new kakao.maps.Map(container, options);

      for (i = 0; i < 10; i++) {
        let markerposition = new kakao.maps.LatLng(
          splicedarray[i].lat,
          splicedarray[i].lng
        );

        let customcontent =
          '<div class="wrap">' +
          '<div class="contentbox" onclick="closeOverlay()" title="닫기">' +
          `<div id="title${i}">` +
          `${splicedarray[i].name}` +
          "</div>" +
          '<div class="body">' +
          '<div class="img">' +
          "</div>" +
          '<div class="desc">' +
          `<span id="contentnumber">number: ${splicedarray[i].number}</span><br />` +
          `<span id="contentshared">shared: ${splicedarray[i].shared}</span>` +
          "</div>" +
          "</div>" +
          "</div>" +
          "</div>";

        let marker = new kakao.maps.Marker({
          position: markerposition,
        });

        let overlay = new kakao.maps.CustomOverlay({
          map: map,
          position: markerposition,
          content: customcontent,
          yAnchor: 1,
        });

        function closeOverlay() {
          overlay.setMap(null);
        }

        kakao.maps.event.addListener(marker, "click", function () {
          overlay.setMap(map);
        });

        let nctct = document.querySelector(`#title${i}`);
        let sharedpercent = parseInt(splicedarray[i].shared);

        if (sharedpercent >= 100) {
          nctct.classList.add("red");
          console.log("red");
        } else {
          if (sharedpercent >= 80) {
            nctct.classList.add("tomato");
            console.log("tomato");
          } else {
            if (sharedpercent >= 60) {
              nctct.classList.add("orange");
              console.log("orange");
            } else {
              if (sharedpercent >= 40) {
                nctct.classList.add("plum");
                console.log("plum");
              } else {
                if (sharedpercent >= 20) {
                  nctct.classList.add("green");
                  console.log("green");
                } else {
                  nctct.classList.add("white");
                  console.log("white");
                }
              }
            }
          }
        }

        marker.setMap(map);
      }

      console.log(distarray);
      console.log(arrangeddistdarray);
      console.log(splicedarray);

      function getdist() {
        const lat1 = matchedstationinform.stationLatitude;
        const lng1 = matchedstationinform.stationLongitude;
        const lat2 = datarow[i].stationLatitude;
        const lng2 = datarow[i].stationLongitude;

        function deg2rad(deg) {
          return deg * (Math.PI / 180);
        }
        let r = 6371; //지구의 반지름(km)
        let dLat = deg2rad(lat2 - lat1);
        let dLon = deg2rad(lng2 - lng1);
        let a =
          Math.sin(dLat / 2) * Math.sin(dLat / 2) +
          Math.cos(deg2rad(lat1)) *
            Math.cos(deg2rad(lat2)) *
            Math.sin(dLon / 2) *
            Math.sin(dLon / 2);
        let c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        let d = r * c; // Distance in km
        return Math.round(d * 1000);
      }
    });
}

function findstation() {
  for (i = 0; i < 1001; i++) {
    if (datarow[i].stationId == `ST-${stationID}`) {
      return true;
    }
  }
}

//지도 생성 및 객체 리턴

submitbutton.addEventListener("click", matchwithAPI);
