<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon>
      $language
    </template>

    <template #headline>
      <template v-if="currentStep === 0">
        Select Your Country or Region
      </template>
      <template v-else-if="currentStep === 1">
        Select Your Timezone
      </template>
    </template>

    <template v-if="currentStep === 0">
      <!-- country -->
      <v-list class="pa-0">
        <template v-for="(country, code) in availableCountries">
          <div :key="code">
            <v-divider />

            <v-list-item
              class="px-0"
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
    </template>

    <template v-else-if="currentStep === 1">
      <!-- timezone if needed -->

      <v-list class="pa-0">
        <template v-for="(zone, i) in availableTimeZones">
          <div :key="i">
            <v-divider />

            <v-list-item
              class="px-0"
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
    </template>
  </setup-panel>
</template>

<script>
  import { mapState, mapActions, mapMutations } from 'vuex'
  import { countries } from 'countries-list'
  import timezones from 'countries-and-timezones'
  import SetupPanel from '@/components/Setup/BasePanel'

  export default {
    components: { SetupPanel },
    data: () => ({
      currentStep: 0,

      availableCountries: countries,
      availableTimeZones: [],
    }),
    computed: {
      ...mapState([
        'stats',
        'settings',
      ]),
    },

    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),

      commitCountry (code, country) {
        this.updateSettings({
          system: {
            country: code,
            language: country.languages[0],
          },
        })

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
        const timezone = timezones.getTimezone(zone)

        this.updateSettings({
          system: {
            timezone: zone,
            utc: timezone.utcOffset * 60,
            dst: timezone.dstOffset * 60,
          },
        })

        this.commitStep()
      },

      commitStep () {
        this.saveSettings().then(() => {
          this.$router.push('/setup/wifi')
        })
      },
      stepBack () {
        this.$router.push('/setup/start')
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
