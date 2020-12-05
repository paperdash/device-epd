<template>
  <setup-panel>
    <v-card-title
      class="display-2 font-weight-bold mb-12 px-0 justify-center text-center"
    >
      Hello paperdash
    </v-card-title>

    <v-sheet
      width="60%"
      class="mx-auto"
    >
      <svg-case id="device" />
    </v-sheet>

    <template #actions>
      <v-btn
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
  import SetupPanel from '@/components/SetupPanel'
  const SvgCase = () => import('@/assets/case_editor.svg?inline')

  export default {
    components: { SetupPanel, SvgCase },
    methods: {
      commitStep () {
        this.$router.push('/setup/country')
      },
    },
  }
</script>

<style scoped>
#device #draft {
  fill: #fff;
}
</style>
