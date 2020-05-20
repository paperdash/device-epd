<template>
  <v-card flat>
    <v-toolbar dark color="primary">
      <v-btn icon dark @click="$emit('cancel')">
        <v-icon>$close</v-icon>
      </v-btn>
      <v-toolbar-title class="pl-0">Enter the password for "{{ ssid }}"</v-toolbar-title>

      <v-progress-linear
        :active="isConnecting"
        indeterminate
        absolute
        bottom
        color="deep-orange accent-4"
      ></v-progress-linear>
    </v-toolbar>

    <v-card-text class="pa-5">
      <v-text-field
        v-model="password"
        :append-icon="show1 ? '$visibility' : '$visibility_off'"
        :type="show1 ? 'text' : 'password'"
        label="i8n:Password"
        @click:append="show1 = !show1"
      ></v-text-field>
    </v-card-text>

    <v-card-actions>
      <v-spacer></v-spacer>
      <v-btn
        depressed
        :disabled="password === ''"
        color="primary"
        @click="onConnect(ssid, password)"
      >i8n:Join</v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
export default {
  props: {
    ssid: {
      type: String,
      required: true
    },
    onConnect: {
      type: Function,
      required: true
    }
  },
  data: () => ({
    isConnecting: false,
    password: "",
    show1: false
  })
};
</script>
