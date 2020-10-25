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
          <div class="justify-center text-center">
            <v-icon
              view-box="0 0 24 24"
              style="width: 64px; height: 64px; fill: #FF9800"
            >
              $face
            </v-icon>
          </div>
          <v-card-title class="display-2 mb-12 justify-center text-center">
            Give it a name
          </v-card-title>

          <p
            class="text-center"
          >
            TODO:Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam
          </p>

          <v-skeleton-loader
            v-if="isLoading"
            type="list-item-two-line"
            class="mx-auto"
          />

          <template v-else>
            <v-card-text>
              <v-text-field
                v-model="settings.device.name"
                label="i8n:My paperdash name"
              >
                <template #append-outer>
                  <v-icon
                    view-box="0 0 24 24"
                    style="width: 48px; height: 48px;"
                    @click="setRandomeName()"
                  >
                    $autorenew
                  </v-icon>
                </template>
              </v-text-field>
            </v-card-text>

            <v-card-actions class="flex-column">
              <v-btn
                :disabled="!isStepValid"
                :loading="isSaving"
                depressed
                block
                color="primary"
                @click="commitStep()"
              >
                Continue
              </v-btn>
            </v-card-actions>
          </template>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  import apiDevice from '@/api/device'
  import randomNames from '@/assets/fantasyNames.json'

  export default {
    data: () => ({
      isLoading: true,
      isSaving: false,
      settings: null,
    }),
    computed: {
      isStepValid () {
        return (
          this.settings.device.name !== undefined &&
          this.settings.device.name !== ''
        )
      },
    },
    created () {
      apiDevice.getSettings(settings => {
        this.settings = settings

        if (!this.isStepValid) {
          this.setRandomeName()
        }

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
        this.$router.push('/setup/appearance')
      },
      setRandomeName () {
        this.settings.device.name =
          randomNames[Math.floor(Math.random() * randomNames.length)]
      },
    },
  }
</script>

<style scoped>
>>> .v-text-field input {
  font-size: 2.2em;
  max-height: inherit;
}
</style>
