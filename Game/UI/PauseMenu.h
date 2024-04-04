// Author: Jake Rieger
// Created: 4/3/2024.
//

#pragma once

namespace PauseMenu {
    static const std::string Code = R""(<rml>
<head>
	<title>Window</title>
	<style>
		body {
			width: 100vw;
			height: 100vh;
			margin: auto;
            font-family: SquareFont;
            font-size: 20dp;
		}

        .background {
            background-color: #000000;
            opacity: 0.5;
			width: 100vw;
			height: 100vh;
            text-align: center;
            position: fixed;
            top: 0;
            left: 0;
            z-index: -1;
        }

        .container {
            display: flex;
            align-items: center;
            justify-content: center;
            flex-direction: column;
            height: 100%;
        }

        h1 {
            font-size: 50dp;
        }
	</style>
</head>
<body class="window">
    <div class="background"/>
    <div class="container">
        <h1>PAUSED</h1>
        <p>Press <b>[ESC]</b> to resume</p>
    </div>
</body>
</rml>)"";
}