<body>
    <h1>Saban Web</h1>
    <p><strong>Saban Web</strong> is a custom web browser and web application framework developed using GTK and C. This project aims to provide a tailored browsing experience with support for various media types and interactive features.</p>
    <h2>Project Overview</h2>
    <ul>
        <li><strong>Custom Browser</strong>: A browser built with GTK and C for a customized browsing experience.</li>
        <li><strong>Media Handling</strong>: Supports displaying images and managing video playback.</li>
        <li><strong>Interactive Elements</strong>: Includes buttons, links, and other interactive components.</li>
    </ul>
    <h2>Key Features</h2>
    <ul>
        <li><strong>Media Display</strong>: Handles images and videos with custom scaling and playback.</li>
        <li><strong>Interactive Tags</strong>: Supports interactive elements like buttons and links.</li>
        <li><strong>Custom Tag Processing</strong>: Parses and handles HTML-like tags for dynamic content.</li>
    </ul>
    <h2>Upcoming Features</h2>
    <p>Integration of socket functionality to enable real-time communication between users in the browser.</p>
    <h2>Tag Overview</h2>
    <h3>Text Tags</h3>
    <ul>
        <li><strong>&lt;p&gt;</strong>: Defines a paragraph.
            <pre><code>&lt;p&gt;This is a paragraph.</code></pre>
            <p><em>Note:</em> The closing tag </p> is not required for proper display and may be omitted if not necessary.</p>
        </li>
        <li><strong>&lt;h1&gt;, &lt;h2&gt;, &lt;h3&gt;</strong>: Defines headers of varying sizes.
            <pre><code>&lt;h1&gt;This is a large header
&lt;h2&gt;This is a medium header
&lt;h3&gt;This is a small header</code></pre>
        </li>
    </ul>
    <h3>Interactive Tags</h3>
    <ul>
        <li><strong>&lt;button&gt;</strong>: Creates a clickable button.
            <pre><code>&lt;button&gt;Click Me</code></pre>
        </li>
        <li><strong>&lt;link&gt;</strong>: Defines a hyperlink.
            <pre><code>&lt;link location=raviv.rizz&gt;Visit Site</code></pre>
            <p><em>Note:</em> The link works as follows: <code>&lt;link location=domain&gt;text</code></p>
        </li>
    </ul>
    <h3>Media Tags</h3>
    <ul>
        <li><strong>&lt;image&gt;</strong>: Displays an image.
            <pre><code>&lt;image src=http://example.com/path/to/image.jpg width=100 height=100&gt;</code></pre>
            <p><em>Note:</em> The <code>src</code> attribute should use an HTTP or HTTPS URL. Quotes around the URL are optional.</p>
        </li>
        <li><strong>&lt;video&gt;</strong>: Embeds a video.
            <pre><code>&lt;video src=http://example.com/path/to/video.mp4 width=320 height=240&gt;</code></pre>
            <p><em>Note:</em> The <code>src</code> attribute should use an HTTP or HTTPS URL. Quotes around the URL are optional.</p>
        </li>
    </ul>
    <h2>Project Status</h2>
    <p>The Saban Web project is currently under development and not yet finished. The existing implementation provides foundational features and serves as a starting point for further development.</p>
</body>
</html>
