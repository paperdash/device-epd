<template>
  <v-container
    class="_fill-height"
    fluid
  >
    <v-row
      no-gutters
      justify="center"
    >
      <v-col
        lg="5"
        md="6"
        sm="8"
      >
        <v-card flat>
          <v-card-title class="display-2 mb-12 justify-center text-center">
            Appearance
          </v-card-title>

          <v-radio-group
            v-model="settings.device.theme"
            row
          >
            <v-radio
              label="Light"
              value="white"
            />
            <v-radio
              label="Dark"
              value="black"
            />
          </v-radio-group>

          <v-card-actions>
            <v-btn
              depressed
              block
              color="primary"
              @click="commitStep()"
            >
              Continue
            </v-btn>
          </v-card-actions>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  import apiDevice from '@/api/device'

  export default {
    data: () => ({
      isLoading: true,
      isSaving: false,
      settings: null,
    }),
    created () {
      apiDevice.getSettings(settings => {
        this.settings = settings

        this.isLoading = false
      })
    },
    methods: {
      commitStep () {
        this.isSaving = true

        apiDevice.putSettings({ device: this.settings.device }, () => {
          this.isSaving = false

          this.nextStep()
        })
      },
      nextStep () {
        this.$router.push('/setup/done')
      },
    },
  }
</script>
