<template>
  <v-container fluid _fill-height>
    <v-row no-gutters justify="center">
      <v-col lg="5" md="6" sm="8">
        <v-card flat>
          <div class="justify-center text-center">
            <v-icon viewBox="0 0 24 24" style="width: 64px; height: 64px; fill: #FF9800">$wb_sunny</v-icon>
          </div>
          <v-card-title class="display-2 mb-12 justify-center text-center">Weather</v-card-title>

          <v-skeleton-loader
            v-if="isLoading"
            type="list-item-two-line,list-item-two-line"
            class="mx-auto"
          ></v-skeleton-loader>

          <template v-else>
            <v-card-text>
              <v-text-field label="i8n:OpenWeatherMap API key" v-model="settings.weather.api">
                <template v-slot:append-outer>
                  <v-icon @click="registerApiKey()">$open_in_new</v-icon>
                </template>
              </v-text-field>

              <weather-find-location
                :api="settings.weather.api"
                :location.sync="settings.weather.location"
                :lang="lang"
                :unit="unit"
              ></weather-find-location>
            </v-card-text>

            <v-card-actions class="flex-column">
              <v-btn
                :disabled="!isLocationValid"
                depressed
                block
                color="primary"
                @click="commitStep()"
              >Continue</v-btn>
              <v-btn
                class="ma-0 mt-3"
                text
                block
                color="primary"
                @click="nextStep()"
              >Set Up Later in Settings</v-btn>
            </v-card-actions>
          </template>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import apiDevice from "@/api/device";
import weatherFindLocation from "@/components/WeatherFindLocation";

export default {
  components: {
    weatherFindLocation
  },
  data: () => ({
    isLoading: true,
    settings: null
  }),
  created() {
    apiDevice.getSettings(settings => {
      this.settings = settings;

      this.isLoading = false;
    });
  },
  computed: {
    lang() {
      return this.settings.language || "EN";
    },
    unit() {
      return this.settings.language === "EN" ? "" : "metric";
    },
    isLocationValid() {
      return this.settings.weather.location > 0;
    }
  },

  methods: {
    commitStep() {
      // TODO sav

      this.nextStep();
    },
    nextStep() {
      this.$router.push("/setup/name");
    },
    registerApiKey() {
      window.open("http://openweathermap.org/");
    }
  }
};
</script>