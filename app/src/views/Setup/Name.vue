<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon>
      $face
    </template>
    <template #headline>
      Give it a name
    </template>

    <v-card-text>
      <v-text-field
        v-model="form.name"
        label="Name of the device"
      >
        <template #append-outer>
          <v-icon
            class="random-icon mt-5 ml-5"
            @click="setRandomName()"
          >
            $autorenew
          </v-icon>
        </template>
      </v-text-field>
    </v-card-text>

    <template #actions>
      <v-btn
        :disabled="!isStepValid"
        depressed
        block
        color="primary"
        @click="commitStep()"
      >
        Continue
      </v-btn>
    </template>
  </setup-panel>
</template>

<script>
  import { mapState, mapMutations, mapActions } from 'vuex'
  import randomNames from '@/assets/fantasyNames.json'
  import SetupPanel from '@/components/Setup/BasePanel'

  export default {
    components: { SetupPanel },
    data: () => ({
      isProcessing: false,
      form: {
        name: '',
      },
    }),
    computed: {
      ...mapState([
        'settings',
      ]),

      isStepValid () {
        return (
          this.form.name !== ''
        )
      },
    },
    created () {
      this.resetChanges()

      if (!this.isStepValid) {
        this.setRandomName()
      }
    },
    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),

      commitStep () {
        this.isProcessing = true

        this.updateSettings({
          device: {
            name: this.form.name,
          },
        })

        this.saveSettings().then(() => {
          this.nextStep()
        })

        // this.nextStep()
      },
      resetChanges () {
        this.form.name = this.settings.device.name
      },
      nextStep () {
        this.$router.push('/setup/appearance')
      },
      setRandomName () {
        this.form.name =
          randomNames[Math.floor(Math.random() * randomNames.length)]
      },
      stepBack () {
        this.$router.push('/setup/weather')
      },
    },
  }
</script>

<style scoped>
>>> .v-text-field input {
  font-size: 2.2em;
  max-height: inherit;
}

>>> .random-icon > .v-icon__svg {
  transform: scale(2);
}
</style>
