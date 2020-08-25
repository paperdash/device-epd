<template>
  <v-container fluid _fill-height>
    <v-btn v-if="currentStep > 0" text color="primary" class="pl-0" @click="currentStep--">
      <v-icon>$prev</v-icon>Back
    </v-btn>

    <template v-if="currentStep === 0">
      <!-- 1. hello -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title class="display-2 mt-12 justify-center text-center">Hello paperdash</v-card-title>

        <Case
          v-if="0"
          id="device_"
          :class="[device.theme, device.case, device.front, 'case_orange', 'my-12']"
        />

        <svg
          id="device"
          :class="[device.theme, device.case, device.front, 'case_orange front_orange', 'my-12 mx-auto']"
          version="1.0"
          xmlns="http://www.w3.org/2000/svg"
          _width="561px"
          _height="527px"
          viewBox="0 0 5610 5270"
          preserveAspectRatio="xMidYMid meet"
          width="400"
        >
          <!--
					<defs>
						<pattern id="pattern_b" width="100%" height="100%">
							<image x="0" y="0" height="84%" href="/face-weather-b.png"/>
						</pattern>
						<pattern id="pattern_w" width="100%" height="100%">
							<image x="0" y="0" height="84%" href="/face-weather-w.png"/>
						</pattern>
					</defs>
          -->

          <g id="border" fill="#262626" stroke="none">
            <path
              d="M442 5110 c-227 -86 -417 -161 -422 -166 -4 -5 -12 -1063 -16 -2350 -7 -2292 -6 -2342 12 -2352 31 -16 817 -242 844 -242 14 0 1086 158 2383 352 l2358 353 6 25 c9 35 -66 3683 -76 3693 -4 4 -1049 194 -2322 422 -1273 228 -2323 416 -2334 419 -11 2 -206 -67 -433 -154z"
            />
          </g>
          <g fill="#f3f3f3" stroke="none">
            <path
              id="front"
              d="M860 2630 l0 -2611 33 6 c17 3 1080 163 2360 355 1281 191 2330 350 2332 352 6 6 -70 3666 -76 3672 -4 4 -4627 836 -4645 836 -2 0 -4 -1175 -4 -2610z m2378 1721 c1217 -175 2214 -320 2216 -322 7 -6 69 -3179 63 -3185 -8 -9 -4489 -624 -4499 -618 -10 6 -11 4444 0 4444 4 0 1003 -144 2220 -319z m2249 2 c-4 -3 -7 0 -7 7 0 7 3 10 7 7 3 -4 3 -10 0 -14z m60 -2890 c-4 -3 -7 0 -7 7 0 7 3 10 7 7 3 -4 3 -10 0 -14z"
            />
            <path
              id="image"
              d="M1022 4642 c-7 -19 5 -4393 13 -4400 5 -5 4465 605 4472 611 4 4 -59 3161 -63 3166 -3 3 -3200 465 -4387 634 -19 2 -31 -1 -35 -11z"
            />
            <path
              id="case"
              d="M438 5081 c-214 -81 -390 -150 -393 -153 -7 -6 -28 -4658 -22 -4664 2 -2 182 -56 400 -119 218 -64 403 -118 412 -121 13 -6 15 259 15 2600 0 2078 -3 2606 -12 2605 -7 0 -187 -67 -400 -148z"
            />
          </g>
        </svg>

        <v-card-actions>
          <v-btn depressed block color="primary" @click="currentStep = 1">Continue</v-btn>
        </v-card-actions>
      </v-card>
    </template>

    <template v-else-if="currentStep === 1">
      <!-- country -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title
          class="display-2 mb-12 justify-center text-center"
        >Select Your Country or Region</v-card-title>

        <v-list class="ml-5 pa-0">
          <template v-for="(country, code) in countries">
            <div :key="code">
              <v-divider></v-divider>

              <v-list-item class="pl-1" @click="commitCountry(code, country)">
                <!--<v-list-item-icon>{{ country.emoji }}</v-list-item-icon>-->
                <v-list-item-content>{{ country.native }}</v-list-item-content>

                <v-list-item-action>
                  <v-icon>$next</v-icon>
                </v-list-item-action>
              </v-list-item>
            </div>
          </template>
        </v-list>
      </v-card>
    </template>

    <template v-else-if="currentStep === 2">
      <!-- timezone if needed -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title class="display-2 mb-12 justify-center text-center">Select Your Timezone</v-card-title>

        <v-list class="ml-5 pa-0">
          <template v-for="(zone, i) in timeZones">
            <div :key="i">
              <v-divider></v-divider>

              <v-list-item class="pl-1" @click="commitTimezone(zone)">
                <v-list-item-content>{{ zone }}</v-list-item-content>

                <v-list-item-action>
                  <v-icon>$next</v-icon>
                </v-list-item-action>
              </v-list-item>
            </div>
          </template>
          <v-divider></v-divider>
        </v-list>
      </v-card>
    </template>

    <template v-else-if="currentStep === 3">
      <!-- wifi select & reboot -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title class="display-2 mb-12 justify-center text-center">
          Choose a Wi-Fi
          <br />Network
        </v-card-title>

        <v-list class="_ml-5 pa-0">
          <v-list-item-group v-model="settings.wifi">
            <template v-for="(wifi, i) in wifiAvailable">
              <div :key="i">
                <v-divider v-if="i > 0"></v-divider>

                <v-list-item class="px-1" :value="wifi.ssid" @click.stop="wifiConnectModal = true">
                  <v-list-item-content>
                    <v-list-item-title v-text="wifi.ssid"></v-list-item-title>
                    <v-list-item-subtitle v-text="wifi.bssid"></v-list-item-subtitle>
                  </v-list-item-content>

                  <v-list-item-icon>
                    <v-icon class="mx-2" v-if="wifi.secure">$lock</v-icon>
                    <v-icon class="mx-2">{{ wifi.rssi | wifiIcon(0) }}</v-icon>
                    <v-icon class="ml-3">$next</v-icon>
                  </v-list-item-icon>
                </v-list-item>
              </div>
            </template>
          </v-list-item-group>

          <v-divider></v-divider>

          <v-btn text color="primary" class="px-0 my-2">Choose Another Network</v-btn>
        </v-list>

        <v-dialog v-model="wifiConnectModal" max-width="400">
          <setup-wifi-connect
            :ssid="settings.wifi"
            @connected="commitWifi()"
            @cancel="wifiConnectModal = false"
          ></setup-wifi-connect>
        </v-dialog>

        <v-divider></v-divider>
      </v-card>
    </template>

    <template v-else-if="currentStep === 4">
      <!-- appearance -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title class="display-2 mb-12 justify-center text-center">Appearance</v-card-title>

        <v-radio-group v-model="settings.theme" row>
          <v-radio label="Light" value="white"></v-radio>
          <v-radio label="Dark" value="black"></v-radio>
        </v-radio-group>

        <v-card-actions>
          <v-btn depressed block color="primary" @click="currentStep++">Continue</v-btn>
        </v-card-actions>
      </v-card>
    </template>

    <template v-else-if="currentStep === 5">
      <!-- weather -->
      <v-card flat class="mx-auto" width="520">
        <v-card-title class="display-2 mb-12 justify-center text-center">Weather</v-card-title>

        <setup-weather :settings="settings"></setup-weather>

        <v-card-actions class="flex-column">
          <v-btn depressed block color="primary" @click="currentStep++">Continue</v-btn>
          <v-btn class="ma-0 mt-3" text block color="primary">Set Up Later in Settings</v-btn>
        </v-card-actions>
      </v-card>
    </template>

    <template v-else-if="currentStep === 6">
      <!-- completed -->
      <v-card flat class="mx-auto mt-12" width="600">
        <!--<v-card-title class="display-2 mb-12 justify-center text-center">This is Your <br/> paperdash</v-card-title>-->
        <v-card-title class="display-2 mb-12 justify-center text-center">Welcome to paperdash</v-card-title>

        <v-card-actions>
          <v-btn depressed block color="primary" to="/">Get Started</v-btn>
        </v-card-actions>
      </v-card>
    </template>
  </v-container>
</template>

<script>
import apiDevice from "@/api/device";
import { countries } from "countries-list";
import timezones from "countries-and-timezones";
import setupWifiConnect from "@/components/SetupWifiConnect";
import setupWeather from "@/components/SetupWeather";
import Case from "!vue-svg-loader!@/assets/case.svg";

export default {
  components: {
    Case,
    setupWifiConnect,
    setupWeather
  },
  data: () => ({
    // weather + ggf. image pool
    currentStep: 5,
    settings: {
			country: "DE",
			language: "en", // (alpha-2 codes)
      timezone: "Europe/Berlin",
      wifi: null,
      theme: null,
      weather: {
        api: '883b3c87223430d6f3a399645f8ba12b',
        location: null,
        lang: null,
        unit: null
      }
    },

    device: {
      theme: "theme_w",
      case: "case_whitey",
      front: "front_whitey"
    },

    countries: countries,
    timeZones: [],

    wifi: {
      available: [],
      connectSSID: null
    },
    wifiAvailable: [],
    wifiConnectModal: false
  }),
  created() {
    apiDevice.wifiScan(list => {
      this.wifiAvailable = list;

      this.isLoading = false;
    });
  },
  methods: {
    commitCountry(code, country) {
      this.settings.country = code;
      this.settings.language = country.languages[0];

      // get also timezone
      let zone = timezones.getCountry(code);
      if (zone.timezones.length > 1) {
        this.timeZones = zone.timezones;
        this.currentStep++;
      } else {
        this.settings.timezone = zone.timezones[0];
        this.currentStep += 2;
      }
    },

    commitTimezone(zone) {
      this.settings.timezone = zone;
      this.currentStep++;
    },
    commitWifi() {}
  }
};
</script>

<style scoped>
#device #border,
#device > path {
  fill: #343434;
}

/* TODO device theme */
#device.theme_w #image {
  fill: url(#pattern_w);
}
#device.theme_b #image {
  fill: url(#pattern_b);
}

/* front color */
#device.front_blacky #front {
  fill: #000;
}
#device.front_whitey #front {
  fill: #f3f3f3;
}
#device.front_orange #front {
  fill: #ee8816;
}

/* case color */
#device.case_blacky #case {
  fill: #000;
}
#device.case_whitey #case {
  fill: #f3f3f3;
}
#device.case_orange #case {
  fill: #ee8816;
}
</style>
