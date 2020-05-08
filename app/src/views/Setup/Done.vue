<template>
  <v-container fluid _fill-height>
    <v-card flat class="mx-auto" width="540">
      <!--<v-card-title class="display-2 mb-12 justify-center text-center">Welcome to paperdash</v-card-title>-->
      <v-card-title class="display-2 mb-12 justify-center text-center">Hello {{ settings.device.name }}</v-card-title>

      <v-card-actions>
        <v-btn outlined block color="primary" @click="onStart">Let's start</v-btn>
      </v-card-actions>
    </v-card>
  </v-container>
</template>

<script>
import apiDevice from "@/api/device";

export default {
  data: () => ({
    isLoading: true,
    isSaving: false,
    settings: null
  }),
  created() {
    apiDevice.getSettings(settings => {
      this.settings = settings;

      this.isLoading = false;
    });
  },
  methods: {
    onStart() {
      this.$router.push("/");
    }
  }
};
</script>