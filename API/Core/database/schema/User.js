const mongoose = require('mongoose');

const userSchema  = new mongoose.Schema({
    computerId: String,
    decryptionKey : String,
    createdAt: { type: Date, default: Date.now }
})

module.exports = mongoose.model('User', userSchema);