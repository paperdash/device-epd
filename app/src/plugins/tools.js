import Vue from 'vue'

// usage: {{ file.size | prettyBytes }}
Vue.filter('prettyBytes', function (num) {
  // jacked from: https://github.com/sindresorhus/pretty-bytes
  if (typeof num !== 'number' || isNaN(num)) {
    throw new TypeError('Expected a number')
  }

  var exponent
  var unit
  var neg = num < 0
  var units = ['B', 'kB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB']

  if (neg) {
    num = -num
  }

  if (num < 1) {
    return (neg ? '-' : '') + num + ' B'
  }

  exponent = Math.min(Math.floor(Math.log(num) / Math.log(1000)), units.length - 1)
  num = (num / Math.pow(1000, exponent)).toFixed(2) * 1
  unit = units[exponent]

  return (neg ? '-' : '') + num + ' ' + unit
})

/**
 * Vue filter to round the decimal to the given place.
 * http://jsfiddle.net/bryan_k/3ova17y9/
 *
 * @param {String} value    The value string.
 * @param {Number} decimals The number of decimal places.
 */
Vue.filter('round', function (value, decimals) {
  if (!value) {
    value = 0
  }

  if (!decimals) {
    decimals = 0
  }

  value = Math.round(value * Math.pow(10, decimals)) / Math.pow(10, decimals)
  return value
})

/**
 * usage: {{ wifi.rssi | wifiIcon(wifi.secure) }}
 * @param {Number} rssi    rssi value
 * @param {Boolean} secure Is wifi protected
 */
Vue.filter('wifiIcon', function (rssi, secure) {
  let icon = '$signalWifi'

  // strength
  if (rssi >= -67) {
    icon += 4
  } else if (rssi >= -70) {
    icon += 3
  } else if (rssi >= -80) {
    icon += 2
  } else if (rssi >= -90) {
    icon += 1
  } else {
    icon += 0
  }

  // secure
  if (secure !== 0 && rssi >= -90) {
    icon += 'Lock'
  }

  return icon
})
