<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #headline>
      Hello {{ settings.device.name }}
    </template>

    <template #actions>
      <v-btn
        outlined
        block
        color="primary"
        @click="onStart"
      >
        Let's start
      </v-btn>
    </template>
  </setup-panel>
</template>

<script>
  import { mapState } from 'vuex'
  import SetupPanel from '@/components/SetupPanel'

  export default {
    components: { SetupPanel },
    data: () => ({
    }),
    computed: {
      ...mapState([
        'settings',
      ]),
    },
    created () {

    },
    methods: {
      onStart () {
        this.$router.push('/')
      },
      stepBack () {
        this.$router.push('/setup/appearance')
      },
    },
  }
</script>
