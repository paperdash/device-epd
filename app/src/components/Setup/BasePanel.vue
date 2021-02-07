<template>
  <v-container
    class="_fill-height"
    fluid
  >
    <div v-if="back">
      <v-btn
        text
        color="primary"
        class="px-0"
        @click="$emit('back')"
      >
        <v-icon>$prev</v-icon>
        Back
      </v-btn>
    </div>

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
          <div
            v-if="hasIconSlot"
            class="justify-center text-center"
          >
            <v-icon
              class="my-icon ma-10"
              color="primary"
            >
              <slot name="icon" />
            </v-icon>
          </div>

          <v-card-title
            class="display-1 font-weight-bold mb-12 px-0 justify-center text-center"
          >
            <slot name="headline" />
          </v-card-title>
        </v-card>

        <slot />

        <v-card-actions
          v-if="hasActionsSlot"
          class="flex-column mt-16"
        >
          <slot name="actions" />
        </v-card-actions>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  export default {
    name: 'SetupPanel',
    props: {
      back: {
        type: Boolean,
        default: false,
      },
    },
    computed: {
      hasIconSlot () {
        return !!this.$slots.icon
      },
      hasActionsSlot () {
        return !!this.$slots.actions
      },
    },
  }
</script>

<style scoped>
>>> .my-icon > .v-icon__component {
  transform: scale(3);
}
</style>
