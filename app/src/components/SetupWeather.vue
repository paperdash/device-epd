<template>
  <v-card flat>
    <v-card-text>
      <v-text-field
        label="i8n:OpenWeatherMap API key"
        v-model="settings.weather.api"
        placeholder="###"
      ></v-text-field>

      <weather-find-location
        :api="settings.weather.api"
        :location.sync="settings.weather.location"
        :lang="lang"
        :unit="unit"
      ></weather-find-location>
    </v-card-text>
  </v-card>
</template>

<script>
import weatherFindLocation from "@/components/WeatherFindLocation";

export default {
  props: {
    settings: {
      type: Object,
      required: true
    }
  },
  components: {
    weatherFindLocation
  },
  data: () => ({
  }),
  computed: {
    lang() {
      return this.settings.language || "EN";
    },
    unit() {
      return this.settings.language === "EN" ? "" : "metric";
    }
  },
  methods: {
    commit() {}
  }
};
</script>
