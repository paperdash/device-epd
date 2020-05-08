<template>
  <v-container fluid _fill-height>
    <v-card flat class="mx-auto" width="520">
      <v-card-title class="display-2 mb-12 justify-center text-center">Appearance</v-card-title>

      <v-radio-group v-model="settings.device.theme" row>
        <v-radio label="Light" value="white"></v-radio>
        <v-radio label="Dark" value="black"></v-radio>
      </v-radio-group>

      <v-card-actions>
        <v-btn depressed block color="primary" @click="commitStep()">Continue</v-btn>
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
    commitStep() {
			this.isSaving = true;

			apiDevice.putSettings({device: this.settings.device}, () => {
        this.isSaving = false;

        this.nextStep();
			});
    },
    nextStep() {
      this.$router.push("/setup/done");
    }
  }
}
</script>