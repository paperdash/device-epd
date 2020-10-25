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
        <template v-if="currentStep === 0">
          <!-- country -->
          <v-card flat>
            <v-card-title
              class="display-2 mb-12 justify-center text-center"
            >
              Select Your Country or Region
            </v-card-title>

            <v-list class="ml-5 pa-0">
              <template v-for="(country, code) in availableCountries">
                <div :key="code">
                  <v-divider />

                  <v-list-item
                    class="pl-1"
                    @click="commitCountry(code, country)"
                  >
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

        <template v-else-if="currentStep === 1">
          <!-- timezone if needed -->
          <v-card flat>
            <v-card-title class="display-2 mb-12 justify-center text-center">
              Select Your Timezone
            </v-card-title>

            <v-list class="ml-5 pa-0">
              <template v-for="(zone, i) in availableTimeZones">
                <div :key="i">
                  <v-divider />

                  <v-list-item
                    class="pl-1"
                    @click="commitTimezone(zone)"
                  >
                    <v-list-item-content>{{ zone }}</v-list-item-content>

                    <v-list-item-action>
                      <v-icon>$next</v-icon>
                    </v-list-item-action>
                  </v-list-item>
                </div>
              </template>
              <v-divider />
            </v-list>
          </v-card>
        </template>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  import apiDevice from '@/api/device'
  import { countries } from 'countries-list'
  import timezones from 'countries-and-timezones'

  export default {
    data: () => ({
      currentStep: 0,

      isLoading: true,
      isSaving: true,
      settings: null,

      availableCountries: countries,
      availableTimeZones: [],
    }),
    created () {
      apiDevice.getSettings(settings => {
        this.settings = settings

        this.isLoading = false
      })
    },
    methods: {
      commitCountry (code, country) {
        this.settings.system.country = code
        this.settings.system.language = country.languages[0]

        // get also timezone
        const zone = timezones.getCountry(code)
        if (zone.timezones.length > 1) {
          this.availableTimeZones = zone.timezones
          this.currentStep = 1
        } else {
          this.commitTimezone(zone.timezones[0])
        }
      },

      commitTimezone (zone) {
        this.settings.system.timezone = zone

        const timezone = timezones.getTimezone(zone)
        this.settings.system.utc = timezone.utcOffset * 60
        this.settings.system.dst = timezone.dstOffset * 60

        this.commitStep()
      },

      commitStep () {
        this.isSaving = true

        apiDevice.putSettings({ system: this.settings.system }, () => {
          this.isSaving = false

          this.$router.push('/setup/wifi')
        })
      },
    },
  }
</script>

<style scoped>
/* issue https://github.com/vuetifyjs/vuetify/issues/9130 */
>>> .v-card__text,
.v-card__title {
  word-break: normal;
}
</style>
