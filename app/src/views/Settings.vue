<template>
  <v-layout fluid fill-height>
    <template v-if="isLoading">
      <v-overlay :absolute="true" :value="true">
        <v-progress-circular indeterminate size="64"></v-progress-circular>
      </v-overlay>
    </template>

    <v-container>
      <v-snackbar v-model="isSnackbar" :timeout="3000" color="success">i8n:saved</v-snackbar>

      <v-tabs v-model="tab" centered icons-and-text>
        <v-tab>
          Device
          <v-icon>$tv</v-icon>
        </v-tab>
        <v-tab>
          Playlist
          <v-icon>$playlist</v-icon>
        </v-tab>
        <v-tab>
          Weather
          <v-icon>$wb_sunny</v-icon>
        </v-tab>
        <!--
        <v-tab>
          Cloud
          <v-icon>$cloud</v-icon>
        </v-tab>
        -->
      </v-tabs>

      <v-card v-if="settings" class="mx-auto" width="400">
        <v-tabs-items v-model="tab">
          <v-tab-item>
            <v-card-text>
              <v-select
                :items="deviceOrientation"
                v-model="settings.device.angle"
                label="i8n:Orientation"
                placeholder
              ></v-select>

              <v-select
                :items="deviceTheme"
                v-model="settings.device.theme"
                label="i8n:Theme"
                placeholder
              ></v-select>

              <v-dialog v-model="dialogSystemUpdate" max-width="400">
                <template v-slot:activator="{ on }">
                  <v-btn class="my-5" block small outlined color="warning" v-on="on">System update</v-btn>
                </template>
                <v-card :loading="system.updateProgress > 0">
                  <template v-slot:progress>
                    <v-progress-linear
                      v-model="system.updateProgress"
                      :indeterminate="system.updateProgress === 100"
                      height="10"
                    ></v-progress-linear>
                  </template>
                  <v-card-title class="headline">System update</v-card-title>
                  <v-card-text>
                    <!--
										<v-alert border="left" type="warning" outlined>
											Sed in libero ut nibh placerat accumsan. Phasellus leo dolor, tempus non, auctor et
										</v-alert>
                    -->

                    <v-file-input
                      v-model="system.firmware"
                      show-size
                      accept="application/octet-stream"
                      label="Firmware"
                    ></v-file-input>
                  </v-card-text>
                  <v-card-actions>
                    <v-btn text href="https://github.com/paperdash" target="_blank">Get firmware</v-btn>
                    <v-spacer></v-spacer>
                    <v-btn
                      outlined
                      color="warning"
                      ref="firmware"
                      @click="onSystemUpdate()"
                    >Update system</v-btn>

                    <!--
										<v-btn color="green darken-1" text @click="dialogSystemUpdate = false">Disagree</v-btn>
										<v-btn color="green darken-1" text @click="dialogSystemUpdate = false">Agree</v-btn>
                    -->
                  </v-card-actions>
                </v-card>

                <v-dialog :value="system.updateResult !== null" persistent max-width="400">
                  <v-card>
                    <v-card-title class="headline">
                      update result...
                      {{ system.updateResult }}
                    </v-card-title>

                    <v-card-actions>
                      <v-spacer></v-spacer>
                      <v-btn text @click="onUpdateDone()">OK</v-btn>
                    </v-card-actions>
                  </v-card>
                </v-dialog>
              </v-dialog>
            </v-card-text>
          </v-tab-item>
          <v-tab-item>
            <v-card-text class="pb-0">
              <v-row>
                <v-col cols="3" class="text-center mt-2 pb-0">
                  <v-icon>$wb_sunny</v-icon>
                  <br />Forecast
                </v-col>
                <v-col cols="6" class="text-center pb-0">
                  <v-text-field
                    v-model="settings.playlist.timer"
                    label="Switch every"
                    type="number"
                    dense
                    rounded
                    filled
                    suffix="seconds"
                    class="text-right"
                  ></v-text-field>
                </v-col>
                <v-col cols="3" class="text-center mt-2 pb-0">
                  <v-icon>$calendar_today</v-icon>
                  <br />Calendar
                </v-col>
              </v-row>

              <ul class="mt-5" v-if="0">
                <li>calendar</li>
                <li>weather forecast</li>
                <li>unsplash.com</li>
              </ul>
            </v-card-text>
          </v-tab-item>
          <v-tab-item>
            <v-card-text>
              <v-text-field
                label="i8n:OpenWeatherMap API key"
                v-model="settings.weather.api"
                placeholder="###"
              ></v-text-field>

              <weather-find-location
                :api="settings.weather.api"
                :location.sync="settings.weather.location"
                :lang="settings.weather.lang"
                :unit="settings.weather.unit"
              ></weather-find-location>

              <v-select
                :items="weatherLang"
                v-model="settings.weather.lang"
                label="i8n:Lang"
                placeholder
              ></v-select>
              <v-select
                :items="weatherUnit"
                v-model="settings.weather.unit"
                label="i8n:Units"
                placeholder
              ></v-select>
            </v-card-text>
          </v-tab-item>
          <!--
          <v-tab-item>
            <v-card-text>
              <v-select
                :items="deviceMode"
                v-model="settings.cloud.mode"
                label="i8n:Device mode"
                placeholder
              ></v-select>

              <v-text-field
                label="i8n:Cloud server"
                v-model="settings.cloud.url"
                placeholder="i8n:https://"
              ></v-text-field>

              <v-text-field
                label="i8n:Cloud Token"
                v-model="settings.cloud.token"
                placeholder="########-####-####-####-############"
              ></v-text-field>
            </v-card-text>
          </v-tab-item>
          -->
        </v-tabs-items>

        <v-card-actions>
					<v-btn text to="/">i8n:cancel</v-btn>

          <v-spacer></v-spacer>

          <v-btn text color="primary" @click="onSave()">i8n:Save</v-btn>
        </v-card-actions>
      </v-card>
    </v-container>
  </v-layout>
</template>

<script>
import apiDevice from "../api/device";
import weatherFindLocation from "./../components/WeatherFindLocation";
import axios from "axios";

export default {
  name: "Settings",
  components: {
    weatherFindLocation
  },
  data: () => ({
    isLoading: true,
    isSnackbar: false,
    tab: 0,

    settings: null,

    // 0 thru 3 corresponding to 4 cardinal rotations
    deviceOrientation: [
      { text: "Nord", value: 0 },
      { text: "East", value: 1 },
      { text: "South", value: 2 },
      { text: "West", value: 3 }
    ],
    deviceTheme: [
      { text: "Black", value: "black" },
      { text: "White", value: "white" }
    ],
    deviceMode: [
      { text: "Active", value: "active" },
      { text: "Passive", value: "passive" }
    ],

    // @see https://openweathermap.org/current#multi
    weatherLang: [
      { text: "English", value: "en" },
      { text: "Deutsch", value: "de" }
    ],
    weatherUnit: [
      { text: "Imperial", value: "" },
      { text: "Metrisch", value: "metric" }
    ],

    dialogSystemUpdate: false,
    system: {
      firmware: null,
      updateProgress: 0,
      updateResult: null
    }
  }),
  created() {
    this.$vuetify.icons.values.tv = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/tv/baseline.svg"
        )
    };
    this.$vuetify.icons.values.playlist = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/live_tv/baseline.svg"
        )
    }; // slideshow | playlist_play | live_tv | queue_play_next
    this.$vuetify.icons.values.keys = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/insert_link/baseline.svg"
        )
    };
    this.$vuetify.icons.values.cloud = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/cloud/baseline.svg"
        )
    };
    this.$vuetify.icons.values.calendar_today = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/calendar_today/baseline.svg"
        )
    };
    this.$vuetify.icons.values.wb_sunny = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/wb_sunny/baseline.svg"
        )
    };
    this.$vuetify.icons.values.system_update = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/system_update_alt/baseline.svg"
        )
    };
    this.$vuetify.icons.values.file = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/attach_file/baseline.svg"
        )
    };
    this.$vuetify.icons.values.warning = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/warning/baseline.svg"
        )
    };
    this.$vuetify.icons.values.clear = {
      component: () =>
        import(
          /* webpackChunkName: "icons" */ "!vue-svg-loader!@material-icons/svg/svg/clear/baseline.svg"
        )
    };

    apiDevice.getSettings(settings => {
      this.settings = settings;

      this.isLoading = false;
    });
  },
  methods: {
    onSave() {
      this.isLoading = true;

      apiDevice.putSettings(this.settings, data => {
        console.log(data);
        this.isLoading = false;
        this.isSnackbar = true;

        this.$router.push("/");
      });
    },

    onSystemUpdate() {
      let self = this;
      self.system.updateProgress = 0;

      const config = {
        onUploadProgress: function(progressEvent) {
          let percentCompleted = Math.round(
            (progressEvent.loaded * 100) / progressEvent.total
          );

          self.system.updateProgress = percentCompleted;
        }
      };

      let formData = new FormData();
      formData.append("update", this.system.firmware);

      axios
        .post("/update", formData, config)
        .then(response => {
          console.log(response.data);

          self.system.updateProgress = null;
          self.system.updateResult = response.data.success;
        })
        .catch(response => {
          console.log(response);

          self.system.updateProgress = null;
          self.system.updateResult = false;
        });
    },

    onUpdateDone() {
      if (this.system.updateResult === true) {
        window.location = "/";
      } else {
        this.system.firmware = null;
        this.system.updateProgress = 0;
        this.system.updateResult = null;
      }
    }
  }
};
</script>

<style scoped>
>>> .v-input.text-right .v-text-field__slot > input {
  text-align: right;
}
</style>
