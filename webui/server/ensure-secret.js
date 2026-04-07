const fs = require('fs');
const crypto = require('crypto');
const path = require('path');

const envPath = path.join(__dirname, '../.env');

module.exports = function() {
    // List of keys we want to ensure exist
    const keysToEnsure = ['SECRET_KEY', 'JWT_SECRET_KEY'];

    let envContent = '';
    if (fs.existsSync(envPath)) {
        envContent = fs.readFileSync(envPath, 'utf8');
    } else {
        fs.writeFileSync(envPath, '', 'utf8');
    }

    keysToEnsure.forEach(key => {
        // Dynamic Regex: looks for the specific key at the start of a line
        const regex = new RegExp(`^${key}=(.*)$`, 'm');
        const match = envContent.match(regex);

        if (match && match[1]) {
            process.env[key] = match[1].trim();
            console.log(`--- ${key} loaded from .env ---`);
        } else {
            // Key missing: Generate, Set, and Append
            const newSecret = crypto.randomBytes(32).toString('hex');
            process.env[key] = newSecret;

            const secretLine = `\n# Generated automatically\n${key}=${newSecret}\n`;
            fs.appendFileSync(envPath, secretLine, 'utf8');
            // Update envContent string so the next loop knows this key now exists
            envContent += secretLine;
            console.log(`--- Created and stored new unique ${key} ---`);
        }
    });
};
