const fs = require('fs');
const crypto = require('crypto');
const path = require('path');

const envPath = path.join(__dirname, '../.env');

// SECRET_KEY signs the session cookie and JWT_SECRET_KEY signs the bearer
// token that /api/db accepts, so .env must not be readable by other local
// users on the host.
const envMode = 0o600;

// Tighten an .env that already exists: a file created by an earlier release
// is still group/world readable after an upgrade. Only chmod when it is
// actually too permissive, and never fail startup over it - the file may be
// provisioned by root while the WebUI runs as an unprivileged user.
function restrictEnv() {
    try {
        if (fs.statSync(envPath).mode & 0o077) {
            fs.chmodSync(envPath, envMode);
        }
    } catch (err) {
        console.warn(`--- cannot restrict permissions on ${envPath}: ${err.message} ---`);
    }
}

module.exports = function() {
    // List of keys we want to ensure exist
    const keysToEnsure = ['SECRET_KEY', 'JWT_SECRET_KEY'];

    let envContent = '';
    if (fs.existsSync(envPath)) {
        envContent = fs.readFileSync(envPath, 'utf8');
        restrictEnv();
    } else {
        fs.writeFileSync(envPath, '', { encoding: 'utf8', mode: envMode });
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
