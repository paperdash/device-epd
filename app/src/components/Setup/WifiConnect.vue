<template>
  <v-card flat>
    <v-toolbar
      dark
      flat
      color="orange darken-2"
      class="pr-0 mr-0"
    >
      <v-toolbar-title>
        {{ ssid }}
      </v-toolbar-title>
      <v-spacer />
      <v-btn
        icon
        dark
        @click="$emit('cancel')"
      >
        <v-icon>$close</v-icon>
      </v-btn>

      <template
        #extension
      >
        <div class="text-center align-content-center">
          Enter the password
        </div>
      </template>
    </v-toolbar>

    <v-card-text class="pa-5">
      <v-text-field
        v-model="password"
        :append-icon="show ? '$visibility' : '$visibility_off'"
        :type="show ? 'text' : 'password'"
        label="Password"
        @click:append="show = !show"
      />
    </v-card-text>

    <v-card-actions>
      <v-spacer />
      <v-btn
        block
        depressed
        :disabled="password === ''"
        color="primary"
        @click="onConnect(ssid, password)"
      >
        Join
      </v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
  export default {
    props: {
      ssid: {
        type: String,
        required: true,
      },
      onConnect: {
        type: Function,
        required: true,
      },
    },
    data: () => ({
      password: '',
      show: false,
    }),
  }
</script>
